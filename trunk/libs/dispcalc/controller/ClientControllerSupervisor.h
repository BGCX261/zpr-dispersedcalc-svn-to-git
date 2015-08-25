/*
 * ClientControllerSupervisor.h
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 *      		lukaszt
 */

#ifndef _DISPCALC_CLIENTCONTROLLERSUPERVISOR_H_
#define _DISPCALC_CLIENTCONTROLLERSUPERVISOR_H_

#include <iostream>
#include <vector>
#include <stack>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <map>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "../parser/ExprTree.h"
#include "ClientControllerTask.h"
#include "../interthread/Message.h"
#include "../interthread/MessageBox.h"

namespace dispcalc
{

//! Klasa watku-opiekuna
/*
 * Otrzymuje zadanie od ClientControllera, dokonuje jego podzialu,
 * rozdysponowuje je pomiędzy ClientControllerTasks po czym zbiera zwrócone wyniki
 * i dokonuje na nich ostatecznych obliczen
 */
class ClientControllerSupervisor {
public:
	typedef std::list<std::string> T_expr;
	typedef ExprTree T_tree;
	typedef std::list<ExprTree> T_treeList;

	//! Struktura pomocnicza do przechowywania
	//! elementu na stosie wynikow powiazanych z watkami
	struct ThreadResult {
		ThreadResult(const boost::optional<boost::thread::id>& id, const boost::optional<std::string>& result) : \
					threadId_(id), result_(result) {}
		boost::optional<boost::thread::id> threadId_;
		boost::optional<std::string> result_;
	};

	//! Konstruktor
	/*!
		Zachowanie kopii stosu oblicze
		\params s  Stos do podzalu miedzy watki
		\params sv Lista serwerow wraz ze stanem
		\params max_h maxymalna wysokosc drzewa
	 */
	ClientControllerSupervisor(T_expr s, const std::map<std::string, bool>& sv, size_t max_h);

	//! Uruchomienie watku-opiekuna
	void run();

	//! Przypisz od razu drzewo do stosu obliczen koncowych
	// \param drzewo
	void assignResultStack(const T_tree&);

	//! Przerywa operacje
	void abort();

	//! Podzial otrzymanego zadania na mniejsze
	/*!
		Dzieli otrzymane drzewo na mniejsze i zleca wyslanie
		poddrzew innym watkom
	 */
	void dispatchTree();

	//! Synalizacja zakoczenia obliczen
	/*!
		Kiedy watek typu Tast zakonczy zlecone obliczenia
		sygnalizuje to watkawi-opiekunowi uzywajac tej metody.

		\params res  String ktory jest wynikiem
		\params id Identyfikator zakonczonego watku
	 */
	void notify(const std::string& res, const boost::thread::id& id, const unsigned& idx);

	void visit();

	//! Wyslanie wiadomosci do skrzynki globalnej
	void postMessage(const std::string& to, const Message& m) const;

	//! Metoda informuje czy wszystkie wyniki podobliczen sa zebrane
	bool isResultComplete() const;

	//! Ostateczne obliczenie wyr, polegajace na zwinieciu stosu resultStack_
	void finalComputation();

	const std::string& chooseSrv() const;

	//! Wyzwalacz watku
	void operator()() {
		run();
	}
	void setMaxH(const size_t& m) { maxH_ = m; }
	size_t getMaxH() const { return maxH_; }
private:
	T_tree tree_;
	size_t maxH_; ///< przechowuje max wysokosc drzewa
	std::vector<ThreadResult> resultStack_;
	std::map<boost::thread::id, ClientControllerTask*> computers_;
	const std::map<std::string, bool>& servers_;
	boost::thread_group computingThreads_;
	boost::mutex mutex_;
};

}

#endif /* _DISPCALC_CLIENTCONTROLLERSUPERVISOR_H_ */

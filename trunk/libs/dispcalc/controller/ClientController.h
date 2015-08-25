/*
 * ClientController.h
 *
 *  Created on: 2009-01-07
 *      Author: Piotr Doniec
 *      		Lukasz Trzaska
 */

#ifndef _dispcalc_clientcontroller_h_
#define _dispcalc_clientcontroller_h_

#include <queue>
#include <map>
#include <fstream>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

#include "Controller.h"
#include "ControllerAction.h"
#include "ClientControllerActions.h"
#include "ClientControllerSupervisor.h"
#include "../interthread/Message.h"
#include "../interthread/MessageBox.h"
#include "../userinterface/UserInterface.h"

#define DEBUG 1

namespace dispcalc
{
//! Klasa główna - kontroluje wszelkie przepływy,
//! obsługuje komendy użytkownika, tworzy wątki dla zadań
class ClientController : public Controller {
public:
	typedef std::list<std::string> T_Expr;

	friend class ComputeAction;
	friend class ComputeMaxNAction;
	friend class StatusAction;

	ClientController(UserInterface* ui);
	~ClientController();

	//! Glowna metoda kontrolera
	/*!
		Metoda bedaca message-loopem. Odczytuje komunikaty
	    z globalej skrzynki zaadresowane do kontrolera
	 */
	void run();
	void stop();

	/*const*/ UserInterface* getUi() const {
		return ui_;
	}

private:
	void translateMessage(Message*) const;

	//! Rozdysponowuje wiadomosci
	/*!
	    Kieruje wiadomosci do odpowiednich metod celem ich
	    wykonania
	 */
	void dispatchMessage(const Message*);

	//! Umieszcza/pobiera wiadomosc z globalnej skrzynki
	void postMessage(const std::string&, const Message&) const;
	bool getMessage(Message *m);

	//! Rozeslanie zadan do serwerow z zadana max wysokoscia drzewa
	void sendTasks(const T_Expr&, size_t maxH = 2);

	//! Wczytanie serwerow do mapy
	void loadServers(std::istream& );

	void loadServers(const char*);

	//! Oznaczenie serwerow jako dostpene.
	/*!
	    Wyslanie zapytania do serwera. Jesli serwer bedzie nie dostepny
		biblioteka rzuci wyajetek ktory mozna przechwycic i odpowiednio
		obsluzyc
	 */
	bool checkServer(const std::string&) const;

	void checkStatus(const std::vector<std::string>&);

	//! Watki - opiekunowie
	/*!
	    Grupa watkow zawierajaca watki opiekunow. Kazdy
	    watek-opiekun odpowiedzialny jest za jedno wyrazenie
	    matematyczne dostarczone przez uzytkownika
	 */
	boost::thread_group supervisorsThreads_;
	std::map<std::string, ClientControllerSupervisor*> supervisors_;

	boost::thread uiThread_; ///< watek interface'u uzytkownika

	std::map<std::string, bool> servers_; ///< adresy ip dostepnych serwerow <ip, dostepny>
	std::map<std::string, ControllerAction<ClientController>* > actions_; ///< akcje reaguja na typ wiadomosci akcje

	UserInterface* ui_;

	volatile bool programRunning_; ///< flaga oznaczajaca stan programu
};


}

#endif

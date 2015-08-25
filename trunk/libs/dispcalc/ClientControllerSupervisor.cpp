/*
 * ClientControllerSupervisor.cpp
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */
#include <sstream>
#include <stack>
#include <list>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include "controller/ClientControllerSupervisor.h"
#include "parser/ExprTree.h"
#include "computer/Computer.h"

using namespace dispcalc;

extern MessageBox<Message> __global_message_box;

ClientControllerSupervisor::ClientControllerSupervisor(T_expr expr,
		const std::map<std::string, bool>& srv, size_t max_h) :
	servers_(srv), maxH_(max_h) {
	tree_.regenerate(expr);
}

void ClientControllerSupervisor::run() {
	if (tree_.height() <= getMaxH()) {
		postMessage("ui", Message(
				"Wysokosc stosu jest za mala by oplacalo sie je przysylac"));

		assignResultStack(tree_);
		finalComputation();
	} else {
		dispatchTree();
	}
}
void ClientControllerSupervisor::postMessage(const std::string& to,
		const Message& m) const {
	__global_message_box.postMessage(to, m);
}
void ClientControllerSupervisor::assignResultStack(const T_tree& tree) {
	T_expr stack = tree.toStack();
	T_expr::const_iterator it = stack.begin();
	for (; it != stack.end(); it++) {
		resultStack_.push_back(ThreadResult(
				boost::optional<boost::thread::id>(), boost::optional<
						std::string>(*it)));
	}
}

void ClientControllerSupervisor::abort()
{
	computingThreads_.interrupt_all();
	std::map<boost::thread::id, ClientControllerTask*>::iterator iter;
	for(iter = computers_.begin(); iter != computers_.end(); iter++)
	{
		delete(iter->second);
	}
	computers_.erase(computers_.begin(), computers_.end());
	postMessage("ui", Message("OBLICZENIA PRZERWANE !!!"));
}

void ClientControllerSupervisor::dispatchTree() {
	size_t maxHeight = getMaxH();
	postMessage("ui", Message("Dispatching tree (to max height "
			+ boost::lexical_cast<std::string>(maxHeight) + ")"));
	std::ostringstream os;
	tree_.print(os);
	postMessage("ui", Message(os.str()));
	os.clear();

	if (maxHeight < 1)
		maxHeight = 1;
	T_treeList chunkedTree = tree_.chunk(maxHeight);
	T_treeList::const_iterator it = chunkedTree.begin();

	postMessage("ui", Message("Tree chunked for calculations to trees:"));
	T_treeList::iterator i1 = chunkedTree.begin();

	while (i1 != chunkedTree.end()) {
		i1->print(os);
		++i1;
	}
	postMessage("ui", Message(os.str()));

	//przydzial poddrzew do watkow
	int idx = 0;
	while (it != chunkedTree.end()) {
		if (!(it->height())) { ///< w elemencie listy drzew wpisany juz wynik lub operator wiec przepisujemy
			resultStack_.push_back(ThreadResult(boost::optional<
					boost::thread::id>(), it->getParent()));
		} else { ///< w elemencie listy drzewo, czyli zadanie do policzenia
			ClientControllerTask* t = new ClientControllerTask(it->toStack(),
					chooseSrv(), this);

			boost::thread* c = computingThreads_.create_thread(boost::ref(*t));

			//dodaj zadanie do listy zadan mapowanych id watku
			computers_.insert(std::make_pair(c->get_id(), t));

			//zapisujemy id watku i niezainicjowany wynik, jako ze go jeszcze nie ma
			resultStack_.push_back(ThreadResult(boost::optional<
					boost::thread::id>(c->get_id()), boost::optional<
					std::string>()));
			t->setIdx(idx); ///< indeks zadania na stosie wynikowym
		}
		++it;
		++idx;
	}
}

void ClientControllerSupervisor::notify(const std::string& result,
		const boost::thread::id& id, const unsigned& idx) {
	boost::interprocess::scoped_lock<boost::mutex> lock(mutex_);
	if (resultStack_[idx].threadId_ != id) {
		postMessage(
				"ui",
				Message(
						"Bledne ID watku, dla danego wezla drzewa, obliczenia przerwane"));
		return;
	}
	resultStack_[idx].result_ = result;
	if (!isResultComplete()) {
		postMessage("ui",
				Message("Supervisor::Oczekiwanie na kompletne wyniki"));
	} else {
		finalComputation();
	}
}

bool ClientControllerSupervisor::isResultComplete() const {
	std::vector<ThreadResult>::const_iterator it = resultStack_.begin();
	for (; it != resultStack_.end(); ++it) {
		if (!(it->result_))
			return false;
	}
	return true;
}

void ClientControllerSupervisor::visit() {
	iqxmlrpc::Client<iqxmlrpc::Http_client_connection>* client;
	std::map<boost::thread::id, ClientControllerTask*>::iterator iter;
	std::vector<std::string> empty;

	for (iter = computers_.begin(); iter != computers_.end(); iter++) {
		ClientControllerTask* ti = iter->second;
		client = new iqxmlrpc::Client<iqxmlrpc::Http_client_connection>(
				iqnet::Inet_addr(ti->getHost(), 8000));
		std::auto_ptr<iqxmlrpc::Client<iqxmlrpc::Http_client_connection> >
				clientap(client);

		iqxmlrpc::Param_list pl;
		iqxmlrpc::Struct s;
		std::stringstream ss;
		ss << iter->first;
		s.insert("id", ss.str());
		pl.push_back(s);

		try {
			iqxmlrpc::Response r = clientap->execute("ServerMethodStatus", pl);
			postMessage("ui", Message(r.value().get_string()));
		} catch (const iqxmlrpc::Exception& e) {
			std::cerr << "Wystapil nie oczekiwany blad biblioteki XML-RPC"
					<< std::endl;
		}
	}
}

void ClientControllerSupervisor::finalComputation() {
	std::stack<int> nums;
	std::vector<ThreadResult>::iterator iter;
	boost::optional<std::string> posOpt;
	std::string res;
	std::list<std::string> countStack;

	postMessage("ui", Message("Supervisor::Obliczenie ostateczne"));
	int i = 0;
	postMessage("ui", Message("Supervisor::Rozmiar stosu "
			+ boost::lexical_cast<std::string>(resultStack_.size())));

	std::ostringstream os;
	for (iter = resultStack_.begin(); iter != resultStack_.end(); iter++) {
		res = *(iter->result_);
		countStack.push_back(res);
	}
	Computer<std::list<std::string>, int> comp(countStack);
	res = comp.compute();
	os << std::endl << "@ Supervisor::Ostateczny wynik(int): " << res
			<< std::endl;
	postMessage("ui", os.str());
}

const std::string& ClientControllerSupervisor::chooseSrv() const {
	std::map<std::string, bool>::const_iterator iter, iterR;
	int random;

	iterR = servers_.begin();
	srand(time(NULL));
	random = rand() % servers_.size();
	for (int i = 0; i < random; i++) {
		++iterR;
	}
	iter = iterR;
	if (!iter->second) {
		while (!iter->second && iter != servers_.end()) {
			++iter;
		}
	}
	if (iter == servers_.end() || !iter->second) {
		while (!iter->second && iter != servers_.begin()) {
			--iter;
		}
	}

	if (!iter->second) {
		postMessage("ui", Message("Can't find server for calculation"));
	}
	return iter->first;
}

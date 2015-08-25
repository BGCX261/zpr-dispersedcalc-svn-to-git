/*
 * ClientControllerTask.h
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */

#ifndef CLIENTCONTROLLERTASK_H_
#define CLIENTCONTROLLERTASK_H_

#include <iostream>
#include <sstream>
#include <vector>

#include <boost/thread/thread.hpp>

#include "../interthread/Message.h"

#include "../libiqxmlrpc/libiqxmlrpc.h"
#include "../libiqxmlrpc/client.h"
#include "../libiqxmlrpc/http_client.h"

namespace dispcalc
{

class ClientControllerSupervisor;
//! Reprezentuje podzadanie, otrzymane od Supervisora po podziale "duzego zadania"
/*
 * Klasa stanowi wątek, odpowiedzialny za obliczenie zadanego podzadania
 * i zwrocenie go supervisorowi calego zadania
 */
class ClientControllerTask {
public:
	typedef std::list<std::string> T_expr;

	ClientControllerTask(const T_expr&, const std::string& host, ClientControllerSupervisor*);

	void run();

	//! Przygotowanie stosu do wyslania
	/*!
		Metoda kowertuje stos z postaci wektora stringow
		na postac kompatyblna z biblioteka
	 */
	iqxmlrpc::Param_list createParamList() const;

	//! Wyzwalacz wtku
	void operator()() {
		run();
	}
	//! posting message via supervisor
	void postMessage(const std::string& to, const Message&) const;

	std::string getHost() const { return host_; }
	void setIdx(const unsigned& i) { idxInResultStack_ = i; }
	unsigned getIdx() const { return idxInResultStack_; }
private:
	//! Reprezentacja polaczenia z serwerem
	iqxmlrpc::Client<iqxmlrpc::Http_client_connection> client_;
	unsigned idxInResultStack_;
	std::string host_;
	T_expr expr_;
	ClientControllerSupervisor* supervisor_;
};

}

#endif /* CLIENTCONTROLLERTASK_H_ */

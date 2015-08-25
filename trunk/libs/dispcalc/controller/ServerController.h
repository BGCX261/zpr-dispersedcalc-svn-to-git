/*
 * ServerController.h
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */

#ifndef _DISPCALC_SERVERCONTROLLER_H_
#define _DISPCALC_SERVERCONTROLLER_H_

#include <map>

#include "Controller.h"
#include "../libiqxmlrpc/libiqxmlrpc.h"
#include "../libiqxmlrpc/http_server.h"
#include "../libiqxmlrpc/executor.h"
#include "../libiqxmlrpc/except.h"
#include "../interthread/Message.h"


namespace dispcalc
{

//std::map<std::string, ComputeController*> operations_;

class ServerController : public Controller
{
public:
	ServerController(int p, int tn) : threadPool_(tn+5)
	{
		server_ = new iqxmlrpc::Http_server(8000, &threadPool_);
	}

	void run();
	void stop();
private:
	void initServer() const;
	void initServiceServer() const;
	void listen() const;

	void postMessage(const std::string& to, const Message&) const {};
	bool getMessage(Message*) {};

	iqxmlrpc::Http_server* server_;
	iqxmlrpc::Pool_executor_factory threadPool_;

};


}

#endif /* _DISPCALC_SERVERCONTROLLER_H_ */

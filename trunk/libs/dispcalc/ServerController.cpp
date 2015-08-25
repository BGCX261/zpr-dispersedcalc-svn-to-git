/*
 * ServerController.cpp
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */

#include "controller/ServerController.h"
#include "server/ServerMethods.h"

using namespace dispcalc;

std::multimap<std::string, std::string> __history;
std::map<std::string, std::string> __actual_status;

void ServerController::run()
{
	initServer();
	listen();
}

void ServerController::stop()
{

}

void ServerController::initServer() const
{

	//computationServer_ = new iqxmlrpc::Http_server(8000, &pf);
	iqxmlrpc::register_method<ServerMethodCompute>(*server_, "ServerMethodCompute");
	iqxmlrpc::register_method<ServerMethodStatus>(*server_, "ServerMethodStatus");
	iqxmlrpc::register_method<ServerMethodConnTest>(*server_, "ServerMethodConnTest");
	server_->log_errors( &std::cerr );
	server_->enable_introspection();
	server_->set_max_request_sz(1024*1024);
}


void ServerController::listen() const
{
	std::cout << "Starting SERVER " << std::endl;
	server_->work();
}

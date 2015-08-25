/*
 * server_test.cpp
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */

#include "libs/dispcalc/controller/ServerController.h"
//#include "libs/dispcalc/server/ServerMethods.h"

int main()
{
//	std::cout << "CCC" << std::endl;;
	dispcalc::ServerController sc(8000, 5);
//	std::cout << "AAA" << std::endl;;
	sc.run();



//	  int port = 8000;
//
//	  iqxmlrpc::Pool_executor_factory pf(2);
//
//	  iqxmlrpc::Http_server server(port, &pf);
//
//	  iqxmlrpc::register_method<dispcalc::ServerMethodCompute>(server, "ServerMethodCompute");
//
//	  // optional settings
//	  server.log_errors( &std::cerr );
//	  server.enable_introspection();
//	  server.set_max_request_sz(1024*1024);
//
//	  // start server
//	  std::cout << "Starting server..." << std::endl;
//	  server.work();
}

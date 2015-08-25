/*
 * ClientControllerTask.cpp
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */

#include "controller/ClientControllerSupervisor.h"
#include "controller/ClientControllerTask.h"
#include "boost/lexical_cast.hpp"
#include "boost/ref.hpp"

using namespace dispcalc;

ClientControllerTask::ClientControllerTask(const T_expr& s, const std::string& host, ClientControllerSupervisor* o) : \
	expr_(s), host_(host), client_(iqnet::Inet_addr(host, 8000)), supervisor_(o)
{
}

void ClientControllerTask::postMessage(const std::string& to, const Message& m) const { supervisor_->postMessage(to, m); }

void ClientControllerTask::run()
{
	iqxmlrpc::Param_list pl = createParamList();
	try
	{
		iqxmlrpc::Response r = client_.execute("ServerMethodCompute", pl);
		postMessage("ui", "Thread(" + boost::lexical_cast<std::string>(boost::this_thread::get_id()) + "): wynik " +  boost::lexical_cast<std::string>(r.value().get_int()));
		supervisor_->notify(boost::lexical_cast<std::string>(r.value().get_int()), boost::this_thread::get_id(), getIdx());
	}
	catch(const iqnet::network_error& e)
	{
		supervisor_->abort();
	}
	catch(const iqxmlrpc::Exception& e)
	{
		postMessage("ui", "Thread(" + boost::lexical_cast<std::string>(boost::this_thread::get_id()) + ") Nieoczekiwany blad lib XML-RPC ");
	}
}

iqxmlrpc::Param_list ClientControllerTask::createParamList() const
{
	iqxmlrpc::Param_list pl;

	T_expr::const_iterator iter;
	pl.push_back(iqxmlrpc::Array());
	for(iter=expr_.begin(); iter!= expr_.end(); iter++) {
		pl[0].push_back(*iter);
	}

	iqxmlrpc::Struct s;
	std::stringstream ss;
	ss << boost::this_thread::get_id();
	s.insert("id", ss.str());
	pl.push_back(s);

	return pl;
}

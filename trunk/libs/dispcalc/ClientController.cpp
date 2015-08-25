/*
 * ClientController.cpp
 *
 *  Author: Piotr Doniec
 *  		Lukasz Trzaska
 */

#include <sstream>
#include "controller/ClientController.h"
#define DEBUG 1

using namespace dispcalc;

extern MessageBox<Message> __global_message_box;


ClientController::ClientController(UserInterface* ui) : programRunning_(true)
{
	ui_ = ui;

	actions_.insert(std::make_pair("exit", new DefaultExitAction(this)));
	actions_.insert(std::make_pair("compute", new ComputeAction(this)));
	actions_.insert(std::make_pair("computemaxn", new ComputeMaxNAction(this)));
	actions_.insert(std::make_pair("status", new StatusAction(this)));
	actions_.insert(std::make_pair("print_warning_no_action", new NoAction(this, std::cout)));
}

ClientController::~ClientController()
{}

void ClientController::run()
{
	bool result;
	Message m;

	loadServers ("serwery.txt");
	uiThread_ = boost::thread(boost::ref(*ui_));
	while(programRunning_)
	{
		{
			result = getMessage(&m);
			if(result)
			{
				translateMessage(&m);
				dispatchMessage(&m);
			}
		}
	}
}

void ClientController::stop()
{
	programRunning_ = false;
}

void ClientController::translateMessage(Message *m) const
{
}

void ClientController::postMessage(const std::string& to, const Message& m) const {
	__global_message_box.postMessage(to, m);
}

bool ClientController::getMessage(Message *m) {
	return __global_message_box.getMessage("controller", m);
}

void ClientController::dispatchMessage(const Message *m)
{
	// wiadomosc zawiera akcje nalezy ja znalezc i wykonac
	const std::string command_name = m->getBody();
	std::map<std::string, ControllerAction<ClientController>* >::iterator iter;

	std::ostringstream os;
	os << "ClientController::dispatchMessage got action (" << command_name << ")";
	postMessage("ui", Message(os.str()));

	iter = actions_.find(command_name);
	if(iter != actions_.end()) {
		((*iter).second)->execute(m->getParams());
	} else {
		iter = actions_.find("print_warning_no_action");
		((*iter).second)->execute();
	}
}

void ClientController::sendTasks(const T_Expr& s, size_t max_h)
{
	ClientControllerSupervisor* st = new ClientControllerSupervisor(s, servers_, max_h);
	boost::thread* nt = supervisorsThreads_.create_thread(boost::ref(*st));

	std::stringstream ss;
	ss << "Identyfikator dostarczonych obliczen to: " << nt->get_id();
	postMessage("ui", Message(ss));

	std::stringstream ss2;
	ss2.flush();
	ss2 << nt->get_id();
	supervisors_.insert(std::make_pair(ss2.str(), st));
}

void ClientController::loadServers(std::istream& in_stream)
{
	std::string line;
	bool result;

	postMessage("ui", Message("loading servers..."));

	unsigned i = 0;
	while(getline(in_stream, line)) {
		boost::trim(line);

		postMessage("ui", Message("(" + line + ")"));
		result = checkServer(line);
		servers_.insert(std::make_pair(line, result));
		++i;
	}
	return;
}
void ClientController::loadServers(const char* filename)
{
	std::ifstream fin( filename );
	return loadServers(fin);
}

bool ClientController::checkServer(const std::string& srv) const
{
	iqxmlrpc::Client<iqxmlrpc::Http_client_connection> testClient(iqnet::Inet_addr(srv, 8000));
	testClient.set_timeout(20);

	postMessage("ui", "@Testowanie serwera: " + srv);

	try
	{
		iqxmlrpc::Response r = testClient.execute("ServerMethodConnTest", "");
		if(r.value().get_int() == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (const iqnet::network_error& e)
	{
	        std::cerr << "@Blad, serwer " << srv << " nie istnieje" << std::endl;
	        return false;
	}
	catch(const iqxmlrpc::Exception& e)
	{
		std::cerr << "@Wystapil nie oczekiwany blad biblioteki XML-RPC" << std::endl;
		exit(-1);
	}

}

void ClientController::checkStatus(const std::vector<std::string>& params)
{
	std::map<std::string, ClientControllerSupervisor*>::iterator iter;

	iter = supervisors_.find(params[0]);
	if(iter != supervisors_.end()) {
		(iter->second)->visit();
	}
	else {
		postMessage("ui", std::string("Niepoprawny identyfikator obliczen"));
	}
}

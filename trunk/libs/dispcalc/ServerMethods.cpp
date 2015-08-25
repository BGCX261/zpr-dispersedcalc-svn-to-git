#include "server/ServerMethods.h"

using namespace dispcalc;

extern std::map<std::string, std::string> __actual_status;
boost::mutex __global_mutex;

void ServerStat::postStat(const std::string& stat, const std::string& ident)
{
	__global_mutex.lock();

	std::map<std::string, std::string>::iterator iter;
	iter = __actual_status.find(ident);
	if(iter != __actual_status.end())
	{
		__actual_status.erase(iter);
	}
	__actual_status.insert(std::make_pair(ident, stat));

	__global_mutex.unlock();
}

void ServerMethodStatus::execute(const iqxmlrpc::Param_list& params, iqxmlrpc::Value& result)
{
	std::string ident;
	ident = params[0]["id"].get_string();

	std::map<std::string, std::string>::iterator iter;
	iter = __actual_status.find(ident);
	if(iter == __actual_status.end())
	{
		result = "Serwer:: Brak stanu wykonania";
	}
	else
	{
		result = iter->second;
	}
}

void ServerMethodCompute::execute(const iqxmlrpc::Param_list& params, iqxmlrpc::Value& result)
{
	std::string pos, mess;
	int num, res, arg1, arg2;

	mess = "Server:: ";
	identifier_ = params[1]["id"].get_string();

	for(int i = 0; i < params[0].size(); i++)
	{
		std::stringstream ss;
		pos = params[0][i].get_string();
		if(isdigit(pos.c_str()[0]) ||  (pos.c_str()[0] == '-' ))
		{
			num = boost::lexical_cast<int>(pos);
			numStack_.push(num);
		}
		else
		{
			arg1 = numStack_.top(); numStack_.pop();
			arg2 = numStack_.top(); numStack_.pop();

			if(pos == "ADD") res = arg1+arg2;
			if(pos == "SUBTRACT") res = arg2-arg1;
			if(pos == "MULTIPLY") res = arg1*arg2;
			if(pos == "DIVIDE")	res = arg2/arg1;
			//if(pos == "NEGATE") { res = -arg1; numStack_.push(arg2);  }

			ss << mess << pos << " arg: " << arg1 << " " << arg2 << " -> " << res;
			postStat(ss.str(), identifier_);	///<rejestruj aktualny stan
			std::cout << ss.str() << std::endl;

			numStack_.push(res);
		}
		sleep(5);
	}

	std::cout << "Server::zwracam wynik " << numStack_.top() << std::endl;
	result = numStack_.top();
}

void ServerMethodConnTest::execute(const iqxmlrpc::Param_list& params, iqxmlrpc::Value& result)
{
	result = 1;
}

/*
 * ClientControllerActions.h
 *
 *  Created on: 2009-01-11
 *      Author: pejotr
 *      		ltrzaska
 */

#include <vector>
#include <boost/lexical_cast.hpp>
#include "controller/ClientControllerActions.h"
#include "parser/Parser.h"

using namespace parser_product;
using namespace dispcalc;

extern MessageBox<Message> __global_message_box;

void DefaultExitAction::execute() {
	(controller_->getUi())->stop();
	controller_->stop();
}

void ComputeAction::execute(const std::vector<std::string>& params) {
	Parser parser;
	int i = 0, j = params.size();
	while (i < j) {
		parse_info<> info = parse(params[i].c_str(), parser, space_p);
		controller_->sendTasks(__expressionStack);
		__expressionStack.clear();
		++i;
	}
}
void ComputeMaxNAction::execute(const std::vector<std::string>& params) {
	Parser parser;
	int maxHeight;
	int i = 0, j = params.size();
	if (j < 2)
		std::cerr << "Bledna liczba parametrow polecenia";
	maxHeight = boost::lexical_cast<int>(params[i++]);
	while (i < j) {
		parse_info<> info = parse(params[i].c_str(), parser, space_p);
		controller_->sendTasks(__expressionStack, maxHeight);
		__expressionStack.clear();
		++i;
	}
}

void StatusAction::execute(const std::vector<std::string>& params) {
	controller_->checkStatus(params);
	//	std::vector<std::string> par;
	//	Message m("Status rozzalony :)", par, "StatusAction", MESSAGE_ACTION );
	//	__global_message_box.postMessage("ui", m);


}

void NoAction::execute() {
	out_ << "warning: unexpected action name" << std::endl;
}

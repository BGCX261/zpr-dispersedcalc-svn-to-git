/*
 * Parser.cpp
 *
 *  Created on: 2009-01-12
 *      Author: pejotr
 */

#include "parser/Parser.h"
#include "boost/lexical_cast.hpp"

namespace parser_product
{
	std::list<std::string> __expressionStack;
}

void    do_int(char const* str, char const* end)
{
	string  s(str, end);  string  all;
	//all += "PUSH("; all += s;  all += ")";
	parser_product::__expressionStack.push_back(s);
}
void    do_add(char const*, char const*)    { parser_product::__expressionStack.push_back("ADD"); }
void    do_subt(char const*, char const*)   { parser_product::__expressionStack.push_back("SUBTRACT"); }
void    do_mult(char const*, char const*)   { parser_product::__expressionStack.push_back("MULTIPLY"); }
void    do_div(char const*, char const*)    { parser_product::__expressionStack.push_back("DIVIDE"); }
void    do_neg(char const*, char const*)    {
	int x = -1 * (boost::lexical_cast<int>(parser_product::__expressionStack.back()));
	parser_product::__expressionStack.pop_back();
	parser_product::__expressionStack.push_back(boost::lexical_cast<std::string>(x));
}

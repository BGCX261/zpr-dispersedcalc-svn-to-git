/*
 * ComputeController.h
 *
 *  Created on: 2009-01-12
 *      Author: lukasz t
 */
#ifndef COMPUTER_H_
#define COMPUTER_H_

#include <stack>
#include <list>
#include <vector>
#include <boost/lexical_cast.hpp>

namespace dispcalc
{

//! Klasa odpowiedzialna za obliczenie wyrazenia zadanego w postaci stosu
//! @param T klasa powinna miec const_iterator
template <typename T, typename NumT>
class Computer {
public:
	Computer(const std::list<std::string>& s) :
		stack_(s) {
	}
	std::string compute() {
		typename T::const_iterator it = stack_.begin();
		std::string pos;
		std::stack<NumT> nums;
		NumT num = 0, arg1 = 0, arg2 = 0, res = 0;
		while (it != stack_.end()) {
			pos = *it;

			if (isdigit(pos.c_str()[0]) || (pos.c_str()[0] == '-')) {
				num = boost::lexical_cast<NumT>(pos);
				nums.push(num);
			} else {
				if (pos == "ADD") {
					arg1 = nums.top();
					nums.pop();
					arg2 = nums.top();
					nums.pop();
					res = arg1 + arg2;
				}
				if (pos == "SUBTRACT") {
					arg1 = nums.top();
					nums.pop();
					arg2 = nums.top();
					nums.pop();
					res = arg2 - arg1;
				}
				if (pos == "MULTIPLY") {
					arg1 = nums.top();
					nums.pop();
					arg2 = nums.top();
					nums.pop();
					res = arg1 * arg2;
				}
				if (pos == "DIVIDE") {
					arg1 = nums.top();
					nums.pop();
					arg2 = nums.top();
					nums.pop();
					res = arg2 / arg1;
				}
//				if (pos == "NEGATE") {
//					arg1 = nums.top();
//					nums.pop();
//					res = -arg1;
//				}
				nums.push(res);
			}
			++it;
		}
		return boost::lexical_cast<std::string>(nums.top());
	}
private:
	T stack_;
};

} //namespace dispcalc

#endif /* COMPUTER_H_ */

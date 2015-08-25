/*
 * ControllerAction.h
 *
 *  Created on: 2009-01-07
 *      Author: Piotr Doniec
 *      		Lukasz Trzaska
 */

#ifndef _DISPCALC_CONTROLLERACTION_H_
#define _DISPCALC_CONTROLLERACTION_H_

namespace dispcalc
{
//! Abstrakcyjna klasa dla wszystkich akcji wykonywanych w aplikacji
template <typename T>
class ControllerAction {
public:
	ControllerAction(T* c) : controller_(c)
	{}

	virtual void execute() {};

	//! execute with params
	virtual void execute(const std::vector<std::string>& ) {};

protected:
	T* controller_;
};

}


#endif /* _DISPCALC_CONTROLLERACTION_H_ */

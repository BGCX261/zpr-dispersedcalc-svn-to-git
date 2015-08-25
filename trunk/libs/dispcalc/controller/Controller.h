/*
 * Controller.h
 *
 *  Created on: 2009-01-08
 *      Author: Piotr Doniec
 *      		ukasz Trzaska
 */

#ifndef _DISPCALC_CONTROLLER_H_
#define _DISPCALC_CONTROLLER_H_

#include "../interthread/Message.h"

namespace dispcalc
{
//! Abstrakcyjna klasa dla kontrolerow
class Controller {
public:
	virtual void run() = 0;
	virtual void stop() = 0;
private:
	virtual void postMessage(const std::string&, const Message&) const = 0;
	virtual bool getMessage(Message*) = 0;
};


}


#endif /* CONTROLLER_H_ */

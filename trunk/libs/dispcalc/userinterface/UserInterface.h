/*
 * UserInterface.h
 *
 *  Created on: 2009-01-08
 *  Author: Piotr Doniec
 *  		Lukasz Trzaska
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "../interthread/Message.h"
#include "../interthread/MessageBox.h"
#include "../controller/Controller.h"

namespace dispcalc
{

//! Abstrakcyjna klasa bazowa interface'u uztkownika
/*!
    Glownym celem utworzenie klasy jest umozliwienia tworzenia
    interface'u uzytkownika opartego o dowolna biblioteke graficzna
    np. Qt czy ncurses. Kazda klasa majaca pelnic role interface'u
    uzytkownika musi implementowac ten interface i definiowac metody
    ktore umozliwia uruchomienie go jako oddzielnego watku.
    Komunikacja z kontrolerem zapewniona jest poprzez wymiane komunikatow.
 */
class UserInterface {
public:
	UserInterface(){}

	virtual void run() {};
	virtual void stop() {};
	virtual void operator()() {};

private:
	virtual void postMessage(Message) {};
	virtual bool getMessage(Message*)  {};

private:

};

}


#endif /* USERINTERFACE_H_ */

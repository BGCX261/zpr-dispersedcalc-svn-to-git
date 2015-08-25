/*
 * UserInterface.h
 *
 *  Created on: 2009-01-07
 *  Author: Piotr Doniec
 *  		Lukasz Trzaska
 */
#include <stdio.h>

#include <iostream>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include "UserInterface.h"

namespace dispcalc
{
//! Tekstowa implementacja interfejsu uzytkownika
class TextUserInterface : public UserInterface {
public:
	TextUserInterface() :outputStream_(std::cout),
						 inputStream_(std::cin),
						 running_(true)
	{}

	void run();

	void stop() {
		printByeMessage();
		running_ = false;
	}

	void setInputStream(std::istream&);
	void setOutputStream(std::ostream&);

	void messageLoop();

	void operator()() {
		printHelloMessage();
		run();
	}

private:
	//! \param message body
	Message createMessage(std::string);

	void postMessage(Message);
	bool getMessage(Message*);

	//! Obsluga polecen otrzymanych od uzytkownika
	/*!
	   TODO: Trzeba by to jakos zgrupowac ze powyzszy komentarz
	   dotyczy wszystkich ponizszych metod :)
	*/
	void checkStatus();
	void exit();

	//! Proste komunikaty :)
	void printHelloMessage() const;
	void printByeMessage() const;

	volatile bool running_;
	std::ostream& outputStream_;
	std::istream& inputStream_;
};

}

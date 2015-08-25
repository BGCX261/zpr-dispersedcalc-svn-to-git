/*
 * UserInterface.h
 *
 *  Created on: 2009-01-08
 *  Author: Piotr Doniec
 *  		Lukasz Trzaska
 */

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "userinterface/TextUserInterface.h"

using namespace dispcalc;

extern MessageBox<Message> __global_message_box;

void TextUserInterface::run()
{
	std::string input;
	Message m;

	boost::thread messageThread(boost::bind(&TextUserInterface::messageLoop, this));
	outputStream_ << "$> ";
	while(running_)
	{
		while( getline(inputStream_, input) ) {
			outputStream_ << "$> ";

			if(!input.empty()) {
				Message m = createMessage(input);
				postMessage(m);
			}
		}
	}
}

void TextUserInterface::printHelloMessage() const
{
	outputStream_ << "*** Rozproszony kalkulator wersja 0.1c ***" << std::endl;
}

void TextUserInterface::printByeMessage() const
{
	outputStream_ << "*** Exiting... ***" << std::endl;
}
//! metoda tworzy wiadomosc do controllera na podstawie komendy usera
Message TextUserInterface::createMessage(std::string mess)
{
	std::vector<std::string> mparams;
	std::string str, mbody;

	typedef boost::tokenizer<boost::char_separator<char> >
	    tokenizer;

	boost::char_separator<char> sep(" ");
	tokenizer tokens(mess, sep);
	unsigned i = 0;
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
		str.clear();
		str.assign(*tok_iter);
		str = boost::trim_copy(str);
		if(0 == i) { mbody = str; ++i; continue; }
		mparams.push_back(str);
	}
	return Message(mbody, mparams, "ui", MESSAGE_ACTION);
}

void TextUserInterface::postMessage(Message m)
{
	__global_message_box.postMessage("controller", m);
}

bool TextUserInterface::getMessage(Message *m)
{
	return __global_message_box.getMessage("ui", m);
}

void TextUserInterface::messageLoop()
{
	Message m;
	while(running_)
	{
		if(getMessage(&m))
		{
			outputStream_ << m.getBody() << std::endl;
			outputStream_ << "$> ";
		}
	}
}

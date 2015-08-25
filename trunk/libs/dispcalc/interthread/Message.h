/*
 * Message.h
 *
 *  Created on: 2009-01-08
 *  Author: Piotr Doniec
 *  		Lukasz Trzaska
 */

#ifndef _DISPCALC_MESSAGE_H_
#define _DISPCALC_MESSAGE_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

namespace dispcalc
{

//! Rodzaj wiadomosci, rozne reakcje kontrolera
enum MessageType { MESSAGE_ACTION, MESSAGE_INFO };

//! Klasa reprezentuje wiadomosc przesylana miedzy watkami
class Message {
public:
	Message() {}
	Message(const std::string& mb, const std::vector<std::string>& params, const std::string& s, MessageType mt) : \
		messageBody_(mb), params_(params), source_(s), type_(mt)
	{}

	Message(const std::string& mb, MessageType mt = MESSAGE_INFO) :
			messageBody_(mb), source_("unknown"), params_(std::vector<std::string>()), type_(mt)
	{}

	Message(const std::stringstream& mb, MessageType mt = MESSAGE_INFO) :
				messageBody_(mb.str()), source_("unknown"), params_(std::vector<std::string>()), type_(mt)
	{}

	MessageType getType() {
		return type_;
	}
	///! Operator rzutowania na string zwraca body
	operator std::string() const {
		return getBody();
	}

	const std::string& getBody() const {
		return messageBody_;
	}

	const std::vector<std::string>& getParams() const {
		return params_;
	}

	unsigned numParams() const {
		return static_cast<unsigned>(params_.size());
	}

private:
	MessageType type_;

	std::vector<std::string> params_;
	std::string source_;
	std::string messageBody_;
};

}

#endif

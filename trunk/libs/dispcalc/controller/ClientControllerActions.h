/*
 * ClientControllerActions.h
 *
 *  Created on: 2009-01-08
 *      Author: pejotr
 *      		lukaszt
 */

#ifndef _DISPCALC_CLIENTCONTROLLERACTIONS_H_
#define _DISPCALC_CLIENTCONTROLLERACTIONS_H_

#include "ClientController.h"
#include "ControllerAction.h"


namespace dispcalc
{

class ClientController;

//! Zakonczenie dzialania programu
class DefaultExitAction : public ControllerAction<ClientController> {
public:
	DefaultExitAction(ClientController *c) : ControllerAction<ClientController>(c) {}
	void execute();
	void execute(const std::vector<std::string>&) { execute(); }
};

//! Odpytanie wszystkich bierzacych obliczen o stan
class StatusAction  : public ControllerAction<ClientController> {
public:
	StatusAction(ClientController *c) : ControllerAction<ClientController>(c) {}
	void execute(){};
	void execute(const std::vector<std::string>&);
};

//! Zlecenie obliczenia wyrazenia matematycznego
class ComputeAction  : public ControllerAction<ClientController> {
public:
	ComputeAction(ClientController *c) : ControllerAction<ClientController>(c) {}
	void execute(const std::vector<std::string>&);
};

//! Zlecenie obliczenia wyrazenia matematycznego
//! z zadana max wysokoscia drzewa
class ComputeMaxNAction  : public ControllerAction<ClientController> {
public:
	ComputeMaxNAction(ClientController *c) : ControllerAction<ClientController>(c) {}
	void execute(const std::vector<std::string>&);
};

//! Wywoylawana jesli rzadana akcja nie zostanie odnaleziona
class NoAction : public ControllerAction<ClientController> {
public:
	NoAction(ClientController *c, std::ostream& s) : ControllerAction<ClientController>(c), out_(s)	{}
	void execute();
	void execute(const std::vector<std::string>&) { execute(); }
private:
	std::ostream& out_;
};

}

#endif

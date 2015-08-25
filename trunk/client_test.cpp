/*
 * client_test.cpp
 *
 *  Created on: 2009-01-09
 *      Author: pejotr
 */

#include "libs/dispcalc/controller/ClientController.h"
#include "libs/dispcalc/userinterface/TextUserInterface.h"
#define DEBUG 1

using namespace dispcalc;

MessageBox<Message> __global_message_box;

int main()
{
	TextUserInterface ui;
	ClientController controller(new TextUserInterface());
	controller.run();
}

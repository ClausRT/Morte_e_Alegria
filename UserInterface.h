/*
 * UserInterface.h
 *
 *  Created on: 8 de mai de 2017
 *      Author: Claus Tessmann
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_
#include <thread>

using namespace std;

class UserInterface {
	bool on;
	thread* tela;
	void telaPrincipal(void);
public:
	UserInterface();
	virtual ~UserInterface();
	void inicia(void);
};

#endif /* USERINTERFACE_H_ */

/*
 * Dado.h
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#ifndef DADO_H_
#define DADO_H_
#include <string>

using namespace std;

class Dado {
public:
	Dado();
	virtual ~Dado();
	string temperatura;
	bool ventoinha;
	bool resistor;
	string data;
	void apagar(void);
};

#endif /* DADO_H_ */

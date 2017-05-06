/*
 * Dado.h
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#ifndef DADO_H_
#define DADO_H_
#include <string>
#include <ctime>

using namespace std;

class Dado {
public:
	Dado();
	virtual ~Dado();
	double temperatura;
	bool ventoinha;
	bool resistor;
	time_t data;
	string dataFormatada(void);	//Pesquisar o real comportamento de uma função estática
};

#endif /* DADO_H_ */

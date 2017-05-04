/*
 * Dado.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include "Dado.h"
#include <ctime>

Dado::Dado() {
	//Só para o eclipse para de incomodar que eu não inicializei nada
	this->ventoinha = false;
	this->resistor = false;
	this->data = 0;
}

Dado::~Dado() {
	// TODO Auto-generated destructor stub
}

string Dado::dataFormatada() {
	return ctime(&this->data);
}

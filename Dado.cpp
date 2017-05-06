/*
 * Dado.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include "Dado.h"
#include <ctime>

Dado::Dado() {
	//S� para o eclipse para de incomodar que eu n�o inicializei nada
	this->ventoinha = false;
	this->resistor = false;
	this->data = 0;
	this->temperatura = 0;
}

Dado::~Dado() {
	// TODO Auto-generated destructor stub
}

string Dado::dataFormatada() {
	string resposta = ctime(&this->data);
	return resposta.substr(0, resposta.length()-1);
}

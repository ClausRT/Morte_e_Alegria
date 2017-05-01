/*
 * Dado.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include "Dado.h"

Dado::Dado() {
	//Só para o eclipse para de incomodar que eu não inicializei nada
	this->ventoinha = false;
	this->resistor = false;
	//Era só isso que eu queria deixar aqui
	this->apagar();
}

Dado::~Dado() {
	// TODO Auto-generated destructor stub
}

void Dado::apagar() {
	this->resistor = false;
	this->ventoinha = false;
	this->data = "";
	this->temperatura = "";
}

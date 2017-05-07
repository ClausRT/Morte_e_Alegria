/*
 * Placa.cpp
 *
 *  Created on: 4 de abr de 2017
 *      Author: Claus Tessmann
 */

#include <string>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <sstream>

#include "Placa.h.ignorar"
#include "temperatura.h"

/**
 * Isso aqui � s� uma mentira
 * Uma classe para testar o programa sem a placa fisica ou simulada no protheus.
 */

/**
 * O construtor exige a porta a qual a placa esta conectada ao computador, pois � necessario para criar o objeto que faz a comunica��o serial.
 * Logo ap�s garante que os tr�s componentes, ventoinha, resistor e lampada, estejam desligadas.
 */
Placa::Placa(int porta) {
	this->_estadoLampada = false;
	this->_estadoResistor = false;
	this->_estadoVentoinha = false;
	this->_serial = NULL;
}

/**
 * O destrutor da placa garante (na verdade ele tenta) que a comunica��o serial seja encerrada corretamente
 */
Placa::~Placa() {
	//this->_serial->fechar();
}

/**
 * Caso o acionamento seja verdadeiro, liga a resistor. E vice-versa.
 * Retorna falso caso a mensagem de retorno da placa n�o corresponda ao acionamento pretendido.
 */
bool Placa::resistor(bool acionamento) {
	this->_estadoResistor = acionamento;
	return acionamento;
}

/**
 * Caso o acionamento seja verdadeiro, liga a ventoinha. E vice-versa.
 * Retorna falso caso a mensagem de retorno da placa n�o corresponda ao acionamento pretendido.
 */
bool Placa::ventoinha(bool acionamento) {
	this->_estadoVentoinha = acionamento;
	return acionamento;
}

/**
 * Caso o acionamento seja verdadeiro, liga a lampada. E vice-versa.
 * Retorna falso caso a mensagem de retorno da placa n�o corresponda ao acionamento pretendido.
 */
bool Placa::lampada(bool acionamento) {
	this->_estadoLampada = acionamento;
	return acionamento;
}

/**
 * Retorna a temperatura atualmente medida pela placa formatado em graus c�lcius com uma casa de presi��o.
 */
string Placa::temperatura(void) {
	srand(time(NULL));
	ostringstream strs;
	strs << ((rand() % 200) + 100.0) / 10.0;
	return strs.str();
}

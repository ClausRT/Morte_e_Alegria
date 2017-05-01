/*
 * Placa.cpp
 *
 *  Created on: 4 de abr de 2017
 *      Author: Claus Tessmann
 */

#include <string>
#include <temperatura.h>
#include "Placa.h"

/**
 * O construtor exige a porta a qual a placa esta conectada ao computador, pois é necessario para criar o objeto que faz a comunicação serial.
 * Logo após garante que os três componentes, ventoinha, resistor e lampada, estejam desligadas.
 */
Placa::Placa(int porta) {
	//Isso é só para o eclipse parar de me incomodar que eu não inicializei as variaveis
	this->_estadoLampada = NULL;
	this->_estadoResistor = NULL;
	this->_estadoVentoinha = NULL;

	this->_serial = new portaSerial();
	_serial.abrir(porta);
	this->resistor(false);
	this->ventoinha(false);
	this->lampada(false);
}

/**
 * O destrutor da placa garante (na verdade ele tenta) que a comunicação serial seja encerrada corretamente
 */
Placa::~Placa() {
	this->_serial.fechar();
}

/**
 * Caso o acionamento seja verdadeiro, liga a resistor. E vice-versa.
 * Retorna falso caso a mensagem de retorno da placa não corresponda ao acionamento pretendido.
 */
bool Placa::resistor(bool acionamento) {
	int envia = (acionamento ? 2 : 4);
	string resposta = this->_serial.enviaSerial(envia);
	if ((acionamento && resposta == "Resistor Ligado")
			|| (!acionamento && resposta == "Resistor Desligado")) {
		this->_estadoResistor = acionamento;
		return true;
	} else
		return false;
}

/**
 * Caso o acionamento seja verdadeiro, liga a ventoinha. E vice-versa.
 * Retorna falso caso a mensagem de retorno da placa não corresponda ao acionamento pretendido.
 */
bool Placa::ventoinha(bool acionamento) {
	int envia = (acionamento ? 3 : 5);
	string resposta = this->_serial.enviaSerial(envia);
	if ((acionamento && resposta == "Ventoinha Ligado")
			|| (!acionamento && resposta == "Ventoinha Desligado")) {
		this->_estadoResistor = acionamento;
		return true;
	} else
		return false;
}

/**
 * Caso o acionamento seja verdadeiro, liga a lampada. E vice-versa.
 * Retorna falso caso a mensagem de retorno da placa não corresponda ao acionamento pretendido.
 */
bool Placa::lampada(bool acionamento) {
	int envia = (acionamento ? 1 : 8);
	string resposta = this->_serial.enviaSerial(envia);
	if ((acionamento && resposta == "Lampada Ligado")
			|| (!acionamento && resposta == "Lampada Desligado")) {
		this->_estadoResistor = acionamento;
		return true;
	} else
		return false;
}

/**
 * Retorna a temperatura atualmente medida pela placa formatado em graus célcius com uma casa de presição.
 */
string Placa::temperatura(void) {
	return this->_serial.enviaSerial(0);
}

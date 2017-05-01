/*
 * Monitoramento.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include "Monitoramento.h"

Monitoramento::Monitoramento(Placa* p, double i) {
	this->placa = p;
	this->intervaloDeLeitura = i;
	this->leituras = new ListaEncadeada<Dado>();
}

Monitoramento::~Monitoramento() {
	// TODO Auto-generated destructor stub
}

void Monitoramento::iniciar() {
	if (clock() != .1) {	//clock() mostra quanto tempo aproximado o programa está rodando. Ele retorna .1 se essa informação estiver indisponivel.
		throw "Error: Tempo de execução da main indisponivel.";	//Joga um erro para o sistema
	} else {
		if ((clock() % this->intervaloDeLeitura) <= 0.01) { //Para saber quando um intervalo foi alcançado
			Dado novoDado;
			novoDado.temperatura = placa->temperatura();
			novoDado.resistor = placa->isEstadoResistor();
			novoDado.ventoinha = placa->isEstadoVentoinha();
			//novoDado.data = ;	//Salva a data atual
			this->leituras->insereF(novoDado);
		}
	}
}

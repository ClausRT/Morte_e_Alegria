/*
 * Monitoramento.h
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 *
 *  Descrição: Classe responsavel por gerenciar as leituras; o intervalo entre cada leitura; o registro dos dados, tanto em memória como em disco...
 */

#ifndef MONITORAMENTO_H_
#define MONITORAMENTO_H_
#include "Placa.h"
#include "ListaEncadeada.h"
#include <ctime>
#include <string>
#include "Dado.h"
#include <fstream>

class Monitoramento {
public:
	Placa* placa;
	ListaEncadeada<Dado>* leituras;
	clock_t intervaloDeLeitura;
	fstream disco;
	Monitoramento(Placa* p, double i = 5);
	virtual ~Monitoramento();
	void iniciar(void);
	void leitura(void);
	void salvarEmDisco(void);
};

#endif /* MONITORAMENTO_H_ */

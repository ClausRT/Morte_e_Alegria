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
#include <thread>
#define INTERVALO_PADRAO 5

class Monitoramento {
	clock_t intervaloDeLeitura;
	thread* leituraContinua;
	Placa* placa;
	ListaEncadeada<Dado> leituras;
	fstream disco;
public:
	Monitoramento(Placa* p, clock_t i = INTERVALO_PADRAO);
	virtual ~Monitoramento();
	void leitura(void);
	void lerContinuamente(bool acionar);
	bool estaLendoContinuamente(void);
	void setIntervaloDeLeitura(clock_t segundos = INTERVALO_PADRAO);
	void salvarEmDisco(void);
	void limparRegistros(void);
};

#endif /* MONITORAMENTO_H_ */

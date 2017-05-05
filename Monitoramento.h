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
	bool haTemMax;
	bool haTemMin;
	double temMax;
	double temMin;
	bool lendoContinuamente;
public:
	Monitoramento(Placa* p, clock_t i = INTERVALO_PADRAO);
	virtual ~Monitoramento();
	void setTemperaturaMaxima(void);
	void setTemperaturaMaxima(double tem);
	void setTemperaturaMinima(void);
	void setTemperaturaMinima(double tem);
	void leitura(void);
	void lerContinuamente(bool acionar);
	bool estaLendoContinuamente(void);
	void setIntervaloDeLeitura(clock_t segundos = INTERVALO_PADRAO);
	void salvarEmDisco(void);
	void limparRegistros(void);
	time_t converteParaTimestamp(unsigned int day, unsigned int month, unsigned int year, unsigned int hour, unsigned int minute, unsigned int second);	// TODO Ainda não sei isso deve ser publico ou privado
	int levantamentoDeOcorrencias (double tem, time_t dataInicial = 0, time_t dataFinal = 0);
	ListaEncadeada<Dado> getLeituras (time_t dataInicial = 0, time_t dataFinal = 0);
	string analise (time_t dataInicial = 0, time_t dataFinal = 0);
	void datalog(int nLeituras, clock_t segundos = INTERVALO_PADRAO, string nome = "datalog_export");
};

#endif /* MONITORAMENTO_H_ */

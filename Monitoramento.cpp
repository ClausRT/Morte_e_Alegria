/*
 * Monitoramento.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include "Monitoramento.h"
#include "ListaEncadeada.h"
#include "Dado.h"
#include <fstream>
#include <iostream>
#define PRECISAO 0.01
#define NOMEDOARQ "leituras.bin"

/**
 * Tirado de http://stackoverflow.com/questions/5055636/casting-an-object-to-char-for-saving-loading
 * A ideia � ter um typecast para usar nos buffers
 */
template<class To, class From>
To any_cast(From v)
{
    return static_cast<To>(static_cast<void*>(v));
}

/**
 * Construtor
 * Precisa que uma Placa que ser� monitorada seja passada como um dos paramentros
 * Opcionalmente � possivel passar um intervalo entre as leituras sendo que o padr�o � 5 unidades de tempo
 * � nesse instante que o arquivo em disco � criado/aberto. Se houver dados a serem lidos, eles ser�o salvos na lista encadeada de leituras
 */
Monitoramento::Monitoramento(Placa* p, double i) {
	this->placa = p;
	this->intervaloDeLeitura = i;
	disco.open(NOMEDOARQ, ios::out | ios::in | ios::binary);//Tenta abrir um arquivo com os dados coletados.
	if (!disco.is_open()) {	//Se n�o conseguir, lan�a um erro no sistema
		throw "Erro ao carregar os dados salvos em disco";
	} else {
		char* buffer = new char[sizeof(Dado)];	//Cria um buffer para um Dado
		disco.seekg(0, ios::beg);	//P�e o ponteiro de leitura no inicio do arquivo (por defini��o ele j� est�ria, mas eu gosto de me precaver)
		while(!disco.eof()) {	//Enquanto n�o achar o final do arquivo, procede com a leitura dos dados, seguido de salvar ele em mem�ria
			disco.read(buffer, sizeof(Dado));
			leituras.insereF(*any_cast<Dado*>(buffer));	//N�o fa�o ideia ainda como converter o buffer em um objeto v�lido
		}
	}
}

/**
 * Destrutor
 * Garante que o arquivo aberto seja fechado
 */
Monitoramento::~Monitoramento() {
	this->disco.close();
}

/**
 * Incompleto
 * Deve iniciar o monitoramento intervalado da placa. Aqui viria a analize de temperatura maxima, minime, ligar ventuinha e todo o resto
 */
void Monitoramento::iniciar() {
	if (clock() != .1) {	//clock() mostra quanto tempo aproximado o programa est� rodando. Ele retorna .1 se essa informa��o estiver indisponivel.
		throw "Error: Tempo de execu��o da main indisponivel.";	//Joga um erro para o sistema
	} else {
		if ((clock() % this->intervaloDeLeitura) <= PRECISAO) { //Para saber quando um intervalo foi alcan�ado

		}
	}
}

/**
 * Realiza a leitura da placa e salva em mem�ria
 */
void Monitoramento::leitura() {
	Dado novoDado;
	novoDado.temperatura = placa->temperatura();
	novoDado.resistor = placa->isEstadoResistor();
	novoDado.ventoinha = placa->isEstadoVentoinha();
	//novoDado.data = ;	//Salva a data atual
	leituras.insereF(novoDado);
}

/**
 * Salva em disco os ultimos dados lidos
 */
void Monitoramento::salvarEmDisco() {
	streampos begin, end;

	disco.seekg(0, ios::beg);	//Coloca o ponteiro de leitura no inicio do arquivo
	begin = disco.tellg();
	disco.seekg(0, ios::end);	//Coloca o ponteiro de leitura no final do arquivo
	end = disco.tellg();

	int dadosEmArquivo = (end - begin) / sizeof(Dado);	//Calcula quantos dados est�o salvos no arquivo

	if (dadosEmArquivo < leituras.getTam()) { //Se houver dados novos, eles ser�o salvos em disco. Como o ponteiro j� se encontra no final do arquivo d� para escrever diretamente
		for (int i = dadosEmArquivo; i < leituras.getTam(); i++) {
			Dado temp = leituras.pos(i);
			disco.write(any_cast<char*>(&temp), sizeof(Dado));	//N�o funciona pois eu ainda tenho que converter Dado para bytes
		}
	}
}

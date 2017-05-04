/*
 * Monitoramento.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include <fstream>
#include <iostream>
#include <ctime>
#include "Monitoramento.h"
#include "ListaEncadeada.h"
#include "ListaEncadeada.cpp"
#include "elemento.h"
#include "Dado.h"
#include "any_cast.cpp"
//#define PRECISAO 0.01
#define NOMEDOARQ "leituras.bin"

/**
 * Construtor
 * Precisa que uma Placa que ser� monitorada seja passada como um dos paramentros
 * Opcionalmente � possivel passar um intervalo entre as leituras sendo que o padr�o � 5 unidades de tempo
 * � nesse instante que o arquivo em disco � criado/aberto. Se houver dados a serem lidos, eles ser�o salvos na lista encadeada de leituras
 */
Monitoramento::Monitoramento(Placa* p, clock_t i) {
	this->placa = p;
	this->intervaloDeLeitura = i;
	this->lerContinuamente = false;
	disco.open(NOMEDOARQ, ios::out | ios::in | ios::binary);//Tenta abrir um arquivo com os dados coletados.
	if (!disco.is_open()) {	//Se n�o conseguir, lan�a um erro no sistema
		throw "Erro ao carregar os dados salvos em disco";
	} else {
		char* buffer = new char[sizeof(Dado)];	//Cria um buffer para um Dado
		disco.seekg(0, ios::beg);	//P�e o ponteiro de leitura no inicio do arquivo (por defini��o ele j� est�ria, mas eu gosto de me precaver)
		while(!disco.eof()) {	//Enquanto n�o achar o final do arquivo, procede com a leitura dos dados, seguido de salvar ele em mem�ria
			disco.read(buffer, sizeof(Dado));
			leituras.insereF(*any_cast<Dado*>(buffer));
		}
	}
}

/**
 * Destrutor
 * Salva os ultimos dados coletados e ent�o fecha o arquivo corretamente
 */
Monitoramento::~Monitoramento() {
	this->salvarEmDisco();
	this->disco.close();
}

/**
 * Incompleto
 * Deve iniciar o monitoramento intervalado da placa. Aqui viria a analize de temperatura maxima, minime, ligar ventuinha e todo o resto

void Monitoramento::iniciar() {
	if (clock() != .1) {	//clock() mostra quanto tempo aproximado o programa est� rodando. Ele retorna .1 se essa informa��o estiver indisponivel.
		throw "Error: Tempo de execu��o da main indisponivel.";	//Joga um erro para o sistema
	} else {
		if ((clock() % this->intervaloDeLeitura) <= PRECISAO) { //Para saber quando um intervalo foi alcan�ado

		}
	}
}*/

/**
 * Realiza a leitura da placa e salva em mem�ria
 */
void Monitoramento::leitura() {
	Dado novoDado;
	novoDado.temperatura = placa->temperatura();
	novoDado.resistor = placa->isEstadoResistor();
	novoDado.ventoinha = placa->isEstadoVentoinha();
	novoDado.data = time(0);	//Salva a data atual em timestamp
	leituras.insereF(novoDado);
}

/**
 * Salva em disco os ultimos dados lidos
 */
void Monitoramento::salvarEmDisco() {
	streampos begin, end;	//tipo de valor v�lido para ponteiros de arquivo

	disco.seekg(0, ios::beg);	//Coloca o ponteiro de leitura no inicio do arquivo
	begin = disco.tellg();
	disco.seekg(0, ios::end);	//Coloca o ponteiro de leitura no final do arquivo
	end = disco.tellg();

	int dadosEmArquivo = (end - begin) / sizeof(Dado);	//Calcula quantos dados est�o salvos no arquivo

	if (dadosEmArquivo < leituras.getTam()) { //Se houver dados novos, eles ser�o salvos em disco. Como o ponteiro j� se encontra no final do arquivo d� para escrever diretamente
		for (int i = dadosEmArquivo; i < leituras.getTam(); i++) {//N�o sei se essa l�gica "pula" algum valor. Eu deveria come�ar em dadosEmArquivo -1 ou tesntar at� <= leituras.getTam()????
			Dado temp = leituras.pos(i);
			disco.write(any_cast<char*>(&temp), sizeof(Dado));
		}
	}
}

/**
 * Seta o tempo entre cada leitura de temperatura.
 * Se o m�todo for chamado sem parametros ele seta a propriedade para o seu valor default
 */
void Monitoramento::setIntervaloDeLeitura(clock_t segundos) {
	this->intervaloDeLeitura = segundos;
}

/**
 * M�todo Set para a variavel lerContinuamente.
 * Ele tamb�m garante que sempre que se altere o estado da leitura a respectiva fun��o que de fato faz a leitura seja chamada. N�o sei at� onde isso seria uma boa ideia.
 */
void Monitoramento::setLerContinuamente(bool estado) {
	this->lerContinuamente = estado;
	this->leituraContinua();
}

bool Monitoramento::getLerContinuamente(void) {
	return this->lerContinuamente;
}

/**
 * M�todo privado cuja fun��o � executar uma leitura continua da temperatura enquanto o
 * lerContinuamente for verdadeiro
 */
void Monitoramento::leituraContinua(void) {
	// TODO E se em vez disso ser uma fun��o, isso ser uma thread
	//e quando se altera o estado de leitura ele simplesmente
	//inicia a thread ou a para, sendo que ela � criada no construtor da classe
}

/**
 * Fecha e reabre o arquivo com no modo Trunk, que acaba deletando de disco o arquivo antigo e assim os dados antigos.
 * Tamb�m destroi todos os elementos da lista de leituras
 */
void Monitoramento::limparRegistros(void) {
	this->disco.close();
	this->disco.open(NOMEDOARQ, ios::out | ios::in | ios::binary);	// TODO falta isso no modo TRUNC
	while(this->leituras.delU());
}

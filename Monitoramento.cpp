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
#include <windows.h>
#define NOMEDOARQ "leituras.bin"

//#include "any_cast.h"	Essa biblioteca não existe mais. Ela não funcionava, por isso me vi obrigado a colar a função aqui.
/**
 * Perdi o link de onde tirei isso, mas era do stackoverflow
 * É usado para transformar uma classe de qualquer tipo em outra.
 * Eu a uso para a conversão de Dado para buffer e vice-versa quando trabalho com arquivos abertos no modo binario
 */
template<class To, class From>
To any_cast(From v)
{
    return static_cast<To>(static_cast<void*>(v));
}

/**
 * Construtor
 * Precisa que uma Placa que será monitorada seja passada como um dos paramentros
 * Opcionalmente é possivel passar um intervalo entre as leituras sendo que o padrão é 5 unidades de tempo
 * É nesse instante que o arquivo em disco é criado/aberto. Se houver dados a serem lidos, eles serão salvos na lista encadeada de leituras
 */
Monitoramento::Monitoramento(Placa* p, clock_t i) {
	//Isso aqui é por que o eclipse estava me enchendo
	this->placa = p;
	this->intervaloDeLeitura = i;
	this->leituraContinua = NULL;
	this->temMax = NULL;
	this->temMin = NULL;
	this->haTemMax = false;
	this->haTemMin = false;

	//Abrindo arquivo salvo em disco
	disco.open(NOMEDOARQ, ios::out | ios::in | ios::binary);//Tenta abrir um arquivo com os dados coletados.
	if (!disco.is_open()) {	//Se não conseguir, lança um erro no sistema
		throw "Erro ao carregar os dados salvos em disco";
	} else {
		char* buffer = new char[sizeof(Dado)];	//Cria um buffer para um Dado
		disco.seekg(0, ios::beg);	//Põe o ponteiro de leitura no inicio do arquivo (por definição ele já estária, mas eu gosto de me precaver)
		while(!disco.eof()) {	//Enquanto não achar o final do arquivo, procede com a leitura dos dados, seguido de salvar ele em memória
			disco.read(buffer, sizeof(Dado));
			leituras.insereF(*any_cast<Dado*>(buffer));
		}
	}
}

/**
 * Destrutor
 * Salva os ultimos dados coletados e então fecha o arquivo corretamente
 */
Monitoramento::~Monitoramento() {
	this->salvarEmDisco();
	this->disco.close();
}

/**
 * Realiza a leitura da placa e salva em memória
 */
void Monitoramento::leitura() {
	Dado novoDado;
	novoDado.temperatura = placa->temperatura();
	novoDado.resistor = placa->isEstadoResistor();
	novoDado.ventoinha = placa->isEstadoVentoinha();
	novoDado.data = time(0);	//Salva a data atual em timestamp

	//Teste dos limiares de temperatura
	if (this->haTemMin && this->temMin <= stof(novoDado.temperatura)) {	// TODO Não sei se é possivel converter desse jeito, mas depois eu testo
		this->placa->resistor(true);
		this->placa->ventoinha(false);
	}
	else if (this->haTemMax && this->temMax <= stof(novoDado.temperatura)) {	// TODO Não sei se é possivel converter desse jeito, mas depois eu testo
		this->placa->ventoinha(true);
		this->placa->resistor(false);
	}

	//Salva o novo dado nos registros
	leituras.insereF(novoDado);
}

/**
 * Salva em disco os ultimos dados lidos
 */
void Monitoramento::salvarEmDisco() {
	streampos begin, end;	//tipo de valor válido para ponteiros de arquivo

	disco.seekg(0, ios::beg);	//Coloca o ponteiro de leitura no inicio do arquivo
	begin = disco.tellg();
	disco.seekg(0, ios::end);	//Coloca o ponteiro de leitura no final do arquivo
	end = disco.tellg();

	int dadosEmArquivo = (end - begin) / sizeof(Dado);	//Calcula quantos dados estão salvos no arquivo

	if (dadosEmArquivo < leituras.getTam()) { //Se houver dados novos, eles serão salvos em disco. Como o ponteiro já se encontra no final do arquivo dá para escrever diretamente
		for (int i = dadosEmArquivo; i < leituras.getTam(); i++) {//Não sei se essa lógica "pula" algum valor. Eu deveria começar em dadosEmArquivo -1 ou tesntar até <= leituras.getTam()????
			Dado temp = leituras.pos(i);
			disco.write(any_cast<char *>(&temp), sizeof(Dado));
		}
	}
}

/**
 * Seta o tempo entre cada leitura de temperatura.
 * Se o método for chamado sem parametros ele seta a propriedade para o seu valor default
 */
void Monitoramento::setIntervaloDeLeitura(clock_t segundos) {
	if (segundos < 0)
		segundos *= -1;
	this->intervaloDeLeitura = segundos;
}

/**
 * Método que avisa se o Monitoramento está lendo continuamente a temperatura.
 * Retorna falso caso não, o que indica que outro método possa usar a porta serial.
 */
bool Monitoramento::estaLendoContinuamente(void) {
	if (!this->leituraContinua) {
		return this->leituraContinua->joinable();
	} else
		return false;
}

/**
 * Método que inicializa e inicia uma thread que faz a leitura continua.
 * A função pode ser chamada novamente para parar a thread/leitura. Importante pois só pode haver uma leitura de temperatura por vez.
 */
void Monitoramento::lerContinuamente(bool acionar) {
	if (acionar) {
		if (this->leituraContinua == NULL) {
			this->leituraContinua = new thread ([this] () {	//Thread iniciada com uma função labda
				while (true) {
					this->leitura();
					Sleep((unsigned int)this->intervaloDeLeitura);	//Não sei se posso fazer isso
				}
			});
		}

		this->leituraContinua->join();
	} else
		delete this->leituraContinua;	// TODO perguntar para o professor se isso é o mais correto
}

/**
 * Fecha e reabre o arquivo com no modo Trunk, que acaba deletando de disco o arquivo antigo e assim os dados antigos.
 * Também destroi todos os elementos da lista de leituras
 */
void Monitoramento::limparRegistros(void) {
	this->disco.close();
	this->disco.open(NOMEDOARQ, ios::out | ios::trunc | ios::in | ios::binary);
	while(this->leituras.delU());
}

/**
 * Método sobrecarregado.
 * Desativa o teste de temperatura máxima — o teste é feito durante a leitura
 */
void Monitoramento::setTemperaturaMaxima(void) {
	this->haTemMax = false;
}

/**
 * Método sobrecarregado.
 * Ativa o teste de temperatura máxima com o valor da temperatura máxima — o teste é feito durante a leitura
 */
void Monitoramento::setTemperaturaMaxima(double tem) {
	this->haTemMax = true;
	this->temMax = tem;
}

/**
 * Método sobrecarregado.
 * Desativa o teste de temperatura mínima — o teste é feito durante a leitura
 */
void Monitoramento::setTemperaturaMinima(void) {
	this->haTemMin = false;
}

/**
 * Método sobrecarregado.
 * Ativa o teste de temperatura minima com o valor da temperatura minima — o teste é feito durante a leitura
 */
void Monitoramento::setTemperaturaMinima(double tem) {
	this->haTemMin = true;
	this->temMin = tem;
}

/**
 * Método para converter a data informada pelo usuario em timestamp
 */
time_t Monitoramento::converteParaData(string formatada) {
// TODO fazer essa conversão corretamente

	return 0;
}

/**
 * Método que retorna quantas vezes uma dada temperatura ocorreu durante um periodo
 * O periodo pode ser omitido para uma análise em todos os dados.
 */
int Monitoramento::levantamentoDeOcorrencias(double tem, time_t dataInicial, time_t dataFinal) {
	//Dado temp = this->leituras.pos(0);
	//time_t temp2 = this->leituras.pos(0);

	time_t inicial = (dataInicial ? this->leituras.pos(0)->data : dataInicial),
			 final = (dataFinal ? this->leituras.pos(this->leituras.getTam()-1) : dataFinal);
	int cont = 0;

	return 0; // temporari TODO remover
}

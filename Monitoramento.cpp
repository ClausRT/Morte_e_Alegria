/*
 * Monitoramento.cpp
 *
 *  Created on: 1 de mai de 2017
 *      Author: Claus Tessmann
 */

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "Monitoramento.h"
#include "ListaEncadeada.h"
#include "ListaEncadeada.cpp"
#include "elemento.h"
#include "Dado.h"
#include <windows.h>
#include <string>
#include <sstream>
#include <windows.h>
#define NOMEDOARQ "leituras.bin"

using namespace std;

//#include "any_cast.h"	Essa biblioteca n�o existe mais. Ela n�o funcionava, por isso me vi obrigado a colar a fun��o aqui.
/**
 * Perdi o link de onde tirei isso, mas era do stackoverflow
 * � usado para transformar uma classe de qualquer tipo em outra.
 * Eu a uso para a convers�o de Dado para buffer e vice-versa quando trabalho com arquivos abertos no modo binario
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
Monitoramento::Monitoramento(Placa* p, clock_t i) {
	//Isso aqui � por que o eclipse estava me enchendo
	this->placa = p;
	this->intervaloDeLeitura = i;
	this->leituraContinua = NULL;
	this->temMax = 0;
	this->temMin = 0;
	this->haTemMax = false;
	this->haTemMin = false;
	this->lendoContinuamente = false;

	//Abrindo arquivo salvo em disco
	disco.open(NOMEDOARQ, ios::out | ios::app | ios::binary);//Tenta abrir um arquivo com os dados coletados.
	disco.close();
	disco.open(NOMEDOARQ, ios::in | ios::binary);
	if (!disco.is_open()) {	//Se n�o conseguir, lan�a um erro no sistema
		cout << "Erro ao carregar os dados salvos em disco" << endl;
	} else {
		char* buffer = new char[sizeof(Dado)];	//Cria um buffer para um Dado
		//disco.seekg(0, ios::beg);	//P�e o ponteiro de leitura no inicio do arquivo (por defini��o ele j� est�ria, mas eu gosto de me precaver)
		while(!disco.eof()) {	//Enquanto n�o achar o final do arquivo, procede com a leitura dos dados, seguido de salvar ele em mem�ria
			disco.read(buffer, sizeof(Dado));
			leituras.insereF(*any_cast<Dado*>(buffer));
		}
	}
	leituras.delP();	//Por algum motivo ele inicializa j� com um primeiro elemento com dados inv�lidos. Por isso apago o primeiro elemento.
	disco.close();
}

/**
 * Destrutor
 * Caso a classe encontre o seu fim antes do programa, garante que as ultimas leituras ser�o salvas em disco
 */
Monitoramento::~Monitoramento() {
	//TODO tem que chamar de algum jeito a fun��o salvarEmDisco antes do encerramento do programa
	this->salvarEmDisco();
	delete this->placa;
}

/**
 * Realiza a leitura da placa e salva em mem�ria
 */
void Monitoramento::leitura() {
	Dado novoDado;
	stringstream temp(placa->temperatura());

	temp >> novoDado.temperatura;
	novoDado.resistor = placa->isEstadoResistor();
	novoDado.ventoinha = placa->isEstadoVentoinha();
	novoDado.data = time(0);	//Salva a data atual em timestamp
	double temperatura = novoDado.temperatura;

	//Teste dos limiares de temperatura
	if (this->haTemMin && this->temMin > temperatura) {
		this->placa->resistor(true);
		this->placa->ventoinha(false);
	}
	else if (this->haTemMax && this->temMax < temperatura) {
		this->placa->ventoinha(true);
		this->placa->resistor(false);
	}
	else {
		this->placa->resistor(false);
		this->placa->ventoinha(false);
	}

	//Salva o novo dado nos registros
	leituras.insereF(novoDado);
}

/**
 * Salva em disco os ultimos dados lidos
 */
void Monitoramento::salvarEmDisco() {
	//streampos begin, end;	//tipo de valor v�lido para ponteiros de arquivo
	Dado temp;
	char* buffer = new char[sizeof(Dado)];
	disco.open(NOMEDOARQ, ios::out | ios::trunc | ios::binary);
	if (!disco.is_open()) {
		cout << "N�o foi possivel abrir o arquivo na fun��o salvarEmDisco" << endl;
		return;
	}/*
	disco.seekp(0, ios::beg);	//Coloca o ponteiro de leitura no inicio do arquivo
	begin = disco.tellp();
	disco.seekp(0, ios::end);	//Coloca o ponteiro de leitura no final do arquivo
	end = disco.tellp();

	int dadosEmArquivo = ((int)end - (int)begin) / sizeof(Dado);	//Calcula quantos dados est�o salvos no arquivo
	cout << "dadosEmArquivo: " << dadosEmArquivo << "  leituras.getTam(): " << leituras.getTam() << endl;
	if (dadosEmArquivo < leituras.getTam()) { //Se houver dados novos, eles ser�o salvos em disco. Como o ponteiro j� se encontra no final do arquivo d� para escrever diretamente
		*/for (int i = 0; i < leituras.getTam(); i++) {//N�o sei se essa l�gica "pula" algum valor. Eu deveria come�ar em dadosEmArquivo -1 ou tesntar at� <= leituras.getTam()????
			temp = leituras.pos(i);
			buffer = any_cast<char *>(&temp);
			disco.write(buffer, sizeof(Dado));
		}
	//}

	//disco.seekg(0, ios::beg); 	//Garante que o ponteiro de leitura esteja no inicio de arquivo caso outra fun��o fa�a uma leitura posteriormente
	disco.close();
}

/**
 * Seta o tempo entre cada leitura de temperatura.
 * Se o m�todo for chamado sem parametros ele seta a propriedade para o seu valor default
 */
void Monitoramento::setIntervaloDeLeitura(clock_t segundos) {
	if (segundos < 0)
		segundos *= -1;
	this->intervaloDeLeitura = segundos;
}

/**
 * M�todo que avisa se o Monitoramento est� lendo continuamente a temperatura.
 * Retorna falso caso n�o, o que indica que outro m�todo possa usar a porta serial.
 */
bool Monitoramento::estaLendoContinuamente(void) {
	return this->lendoContinuamente;
}

/**
 * M�todo que inicializa e inicia uma thread que faz a leitura continua.
 * A fun��o pode ser chamada novamente para parar a thread/leitura. Importante pois s� pode haver uma leitura de temperatura por vez.
 */
void Monitoramento::lerContinuamente(bool acionar) {
	if (acionar) {
		if (this->leituraContinua == NULL) {
			this->leituraContinua = new thread ([this] () {	//Thread iniciada com uma fun��o labda
				while (this->lendoContinuamente) {
					this->leitura();
					Sleep((unsigned int)this->intervaloDeLeitura);	//N�o sei se posso fazer isso
				}
			});
		}

		//this->leituraContinua->join();
	} else {
		//delete this->leituraContinua;	// TODO perguntar para o professor se isso � o mais correto
	}
	lendoContinuamente = acionar;
}

/**
 * Tamb�m destroi todos os elementos da lista de leituras
 * Como o arquivo � sempre aberto em modo trunk no m�todo salvarEmDisco, ent�o os registros antigos ser�o perdidos no proximo salvamento.
 * E a fun��o salvarEmDisco � executada automaticamente quando o destrutor de Monitoramento for chamado.
 */
void Monitoramento::limparRegistros(void) {
	while(leituras.delP());
}

/**
 * M�todo sobrecarregado.
 * Desativa o teste de temperatura m�xima � o teste � feito durante a leitura
 */
void Monitoramento::setTemperaturaMaxima(void) {
	this->haTemMax = false;
}

/**
 * M�todo sobrecarregado.
 * Ativa o teste de temperatura m�xima com o valor da temperatura m�xima � o teste � feito durante a leitura
 */
void Monitoramento::setTemperaturaMaxima(double tem) {
	this->haTemMax = true;
	this->temMax = tem;
}

/**
 * M�todo sobrecarregado.
 * Desativa o teste de temperatura m�nima � o teste � feito durante a leitura
 */
void Monitoramento::setTemperaturaMinima(void) {
	this->haTemMin = false;
}

/**
 * M�todo sobrecarregado.
 * Ativa o teste de temperatura minima com o valor da temperatura minima � o teste � feito durante a leitura
 */
void Monitoramento::setTemperaturaMinima(double tem) {
	this->haTemMin = true;
	this->temMin = tem;
}

/**
 * M�todo para converter a data informada pelo usuario em timestamp
 * N�o estou t�o certo se isso deveria estar aqui ou em outra classe...
 */
time_t Monitoramento::converteParaTimestamp(unsigned int day, unsigned int month, unsigned int year, unsigned int hour, unsigned int minute, unsigned int second) {
	struct tm tmdate = {0};
	tmdate.tm_year = year - 1900;
	tmdate.tm_mon = month - 1;
	tmdate.tm_mday = day;
	tmdate.tm_hour = hour;
	tmdate.tm_min = minute;
	tmdate.tm_sec = second;
	time_t t = mktime( &tmdate );
	return t;
}

/**
 * M�todo utilizado sempre que se quer pegar alguma leitura da mem�ria
 * Como novos dados sempre s�o adicionado ao final da lista Leituras e est� n�o �
 * alterada em nenhum outro momento, � seguro pressumir que Leituras estar� sempre
 * ordenada da data mais antiga at� a mais rescente
 */
ListaEncadeada<Dado> Monitoramento::getLeituras(time_t dataInicial, time_t dataFinal) {
	Dado temp;
	time_t inicial, final;
	ListaEncadeada<Dado> resposta;

	if (!dataInicial && !dataFinal)
		return this->leituras;

	if (dataInicial)
		inicial = dataInicial;
	else {
		temp = this->leituras.pos(0);	//Retorna o primeiro elemento
		inicial = temp.data;
	}

	if (dataFinal)
		final = dataInicial;
	else {
		temp = this->leituras.pos(this->leituras.getTam()-1);	//Retorna o ultimo elemento
		final = temp.data;
	}

	for (int i = 0; i < this->leituras.getTam(); i++) {
		temp = this->leituras.pos(i);
		if (temp.data >= inicial || temp.data <= final) {
			resposta.insereF(temp);
		}
	}

	return resposta;
}

/**
 * M�todo que retorna quantas vezes uma dada temperatura ocorreu durante um periodo
 * O periodo pode ser omitido para uma an�lise em todos os dados.
 */
int Monitoramento::levantamentoDeOcorrencias(double tem, time_t dataInicial, time_t dataFinal) {
	int cont = 0;
	Dado dado;
	ListaEncadeada<Dado> lista = getLeituras(dataInicial, dataFinal);

	for (int i = 0; i < lista.getTam(); i++){
		dado = lista.pos(i);
		if (dado.temperatura == tem)
			cont++;
	}

	return cont;
}

/**
 * M�todo que retorna um vetor (util para a UI) com as medidas estatisticas de temperatura minima, maxima, media e mediana (nessa ordem) de um dado periodo de tempo
 *
 * Professor Andre Geraldo, n�o deu para entender o que o senhor quis dizer com temperatura m�xima e minima no enunciado a seguir:
 * A temperatura m�xima e m�nima, media e mediana ocorrida em um
 * per�odo. Al�m da quantidade de temperatura acima da m�xima, e
 * abaixo da m�nima.
 * N�o sabemos ent�o se era para mostrar a temperatura m�xima que ocorreu no periodo ou a m�xima que o usuario setou no programa
 * (a mesma duvida para temperatura minima). Ent�o fizemos s� metade do enunciado, ignorando a ultima sentenn�a.
 */
double* Monitoramento::analise (time_t dataInicial, time_t dataFinal) {
	ListaEncadeada<Dado> lista = getLeituras(dataInicial, dataFinal);
	Dado temp;
	double *temperaturas = new double [lista.getTam()];
	double cont = 0;

	for (int i = 0; i < lista.getTam(); i++) {
		temp = lista.pos(i);
		temperaturas[i] = temp.temperatura;
		cont += temperaturas[i];
	}

	for (int i = 0; i < lista.getTam(); i++) {
		for (int j = 0; j < lista.getTam(); j++) {
			if (temperaturas[i] > temperaturas [j])
				swap(temperaturas[i], temperaturas[j]);
		}
	}

	cont /= lista.getTam();
	double* resposta = new double[4];
	resposta[0] = temperaturas[lista.getTam()-1];
	resposta[1] = temperaturas[0];
	resposta[2] = cont;
	resposta[3] = temperaturas[lista.getTam()/2];

	return resposta;
}

void Monitoramento::datalog(int nLeituras, clock_t segundos, string nome) {
	int inicial = leituras.getTam(), final = leituras.getTam() + nLeituras;

	thread esperaLeituras([this] (int final, clock_t segundos) {
		while(leituras.getTam() < final) {
			this->leitura();	//Por algum motivo esse thread para o funcionamento do outro thread de leitura, me for�ando a fazer a leitura aqui
			Sleep(segundos);
		}
	}, final, segundos);

	esperaLeituras.join();

	//Salvando em arquivo
	fstream arquivo;
	nome += ".log";
	Dado lido;
	const char* filename = nome.c_str();

	arquivo.open(filename, ios::out | ios::trunc);
	for (int i = inicial; i < final; i++) {
		lido = this->leituras.pos(i);
		arquivo << lido.dataFormatada() << "  " << lido.temperatura << "C" << endl;
	}
	arquivo.close();
}

void Monitoramento::exportarCSV (time_t dataInicial, time_t dataFinal) {
	fstream arquivo;
	ListaEncadeada<Dado> lista = this->getLeituras(dataInicial, dataFinal);
	Dado lido;
	arquivo.open("export.csv", ios::out | ios::trunc);
	arquivo << "Data;Temperatura;Estado do Resistor;Estado da Ventoinha" << endl;
	for (int i = 0; i < lista.getTam(); i++) {
		lido = lista.pos(i);
		arquivo << lido.dataFormatada() << ";" << lido.temperatura << ";" << lido.resistor << ";" << lido.ventoinha << endl;
	}
	arquivo.close();
}
/*
void Monitoramento::plotarGrafico(time_t dataInicial, time_t dataFinal) {
	ofstream arqGNU;
	arqGNU.open("simple.8.gnu");
	arqGNU << "set terminal pngcairo enhanced font \"Verdana,10\" fontscale 1.5 size 1280, 720" << endl;
	arqGNU << "set output 'gnuplot.png'" << endl;
	arqGNU << "set key bmargin left horizontal Right noreverse enhanced autotitles box linetype -1 linewidth 1.000" << endl;
	arqGNU << "set samples 1280,720" << endl;
	arqGNU << "plot 'temperatura.dat' title \"Temperatura em C\" with lines lc rgb \"red\", 'min.dat' title \"Temperatura Min.\" with lines lt 0, 'max.dat' title \"Temperatura Max.\" with lines lt 0" << endl;
	arqGNU.close();

	ListaEncadeada<Dado> lista = getLeituras(dataInicial, dataFinal);
	Dado temp;
	ofstream arqTemp, arqMin, arqMax;
	arqTemp.open("temperatura.dat");
	arqMin.open("min.dat");
	arqMax.open("max.dat");
	for (int i = 0; i < lista.getTam(); i++) {
		temp = lista.pos(i);
		arqTemp << temp.temperatura << endl;
		arqMin << this->temMin << endl;
		arqMax << this->temMax << endl;
	}
	arqTemp.close();
	arqMin.close();
	arqMax.close();

	//Como a plotagem � um programa externo, sua execu��o e espera � feita em uma thread para n�o interromper o resto da execu��o do programa
	thread gnu ([this] () {
		string error;
		try{
			system("gnuplot.exe simple.8.gnu");
			Sleep(3000);
			system("gnuplot.png");
			remove("temperatura.dat");
			remove("min.dat");
			remove("max.dat");
			remove("simple.8.gnu");
		} catch (error) {
			//throw error;
			cout << "Um erro foi achado" << endl;
		}
	});
}*/

#include <cstdio>
#include <iostream>
#include "Monitoramento.h"
#include <windows.h>
#include "Placa_mock.h"

int main(void)
{
//	cout << "Iniciando o programa." << endl;

	Placa* kitPIC = new Placa(3);	//Comunicação serial na COM3
	Monitoramento* monitor = new Monitoramento(kitPIC, 1000); 	//2 segundos entre as leituras

	cout << "Iniciada a leitura da temperatura." << endl;

	monitor->setTemperaturaMaxima(24.2);
	monitor->setTemperaturaMinima(15.8);
	monitor->lerContinuamente(true);
	//monitor->salvarEmDisco();

	ListaEncadeada<Dado> lista = monitor->getLeituras();
	cout << "Numero de leituras: " << lista.getTam() << endl;
	Sleep(10000);
	lista = monitor->getLeituras();
	cout << "Numero de leituras apos 10s: " << lista.getTam() << endl;

	Dado lido;
	for (int i = 0; i < lista.getTam(); i++){
		lido = lista.pos(i);
		cout << lido.temperatura << "  " << lido.dataFormatada() << endl;
	}

	cout << "Leitura terminada." << endl;

	cout << "Esse é o fim do programa." << endl;
    delete monitor;
	return 0;

}

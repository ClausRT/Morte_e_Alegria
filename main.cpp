#include <cstdio>
#include <iostream>
#include "Monitoramento.h"
#include <windows.h>
#include "Placa_mock.h"

int main(void)
{
//	cout << "Iniciando o programa." << endl;

	Placa* kitPIC = new Placa(3);	//Comunicação serial na COM3
	Monitoramento* monitor = new Monitoramento(kitPIC, 1000);

	cout << "Iniciada a leitura da temperatura." << endl;
	monitor->setTemperaturaMaxima(20);
	monitor->setTemperaturaMinima(15);
	monitor->leitura();

	ListaEncadeada<Dado> lista = monitor->getLeituras();
	Dado lido;
	for (int i = 0; i < lista.getTam(); i++){
		lido = lista.pos(i);
		cout << lido.temperatura << " Resistor: " << lido.resistor << " Ventoinha: " << lido.ventoinha << "  " << lido.dataFormatada() << endl;
	}
	cout << "Leituras: " << lista.getTam() << endl;
	cout << "Leitura terminada." << endl;
	cout << "Esse é o fim do programa." << endl;
	monitor->salvarEmDisco();

	return 0;

}

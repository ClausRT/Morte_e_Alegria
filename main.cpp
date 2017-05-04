#include <cstdio>
#include <iostream>
#include "Placa.h"
#include "Monitoramento.h"
#include <windows.h>

int main(void)
{
	Placa* kitPIC = new Placa(3);	//Comunicação serial na COM3
	Monitoramento* monitor = new Monitoramento(kitPIC, 2); 	//2 segundos entre as leituras

	monitor->lerContinuamente(true);
	cout << "Iniciada a leitura da temperatura." << endl;
	cout << "Leitura em andamento..." << endl;
	Sleep(60 * 1000);	//Espera por um minuto - aproximadamente 30 leituras
	monitor->lerContinuamente(false);
	cout << "Leitura terminada." << endl;
    return 0;
}

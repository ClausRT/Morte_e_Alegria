#include <cstdio>
#include <iostream>
#include "Monitoramento.h"
#include <windows.h>
#include "Placa_mock.h"
#include "UI.h"

int main(void) {
	Placa* kitPIC = new Placa(3);	//Comunicação serial na COM3. Samuel, você pode criar um novo menu para a escolha da porta serial. Tudo que é necessario fazer é criar um novo objeto placa e substituir no objeto de classe monitoramento
	Monitoramento* monitor = new Monitoramento(kitPIC, 1000);

	ListaEncadeada<Dado> lista = monitor->getLeituras();
	Dado lido;
	if (true)
		for (int i = 0; i < lista.getTam(); i++) {
			lido = lista.pos(i);
			cout << lido.temperatura << "C \tResistor: " << lido.resistor
					<< " \tVentoinha: " << lido.ventoinha << "  \t"
					<< lido.dataFormatada() << endl;
		}
	cout << "Leituras: " << lista.getTam() << endl;
	//monitor->plotarGrafico();
	cout << "Esse é o fim do programa." << endl;

	return 0;
}

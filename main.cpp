#include <cstdio>
#include <iostream>
#include <string>
#include <windows.h>
#include "temperatura.h"
#include "Placa.h"

int main(void)
{
	//Placa placa = new Placa(3);
    string r;
    portaSerial s; // Cria a classe porta serial
    s.abrir(3); // abre uma conexão na COM3
    cout << s.enviaSerial(0); // Envia o comando zero e guarda a resposta em r
    s.fechar(); // Fecha a serial
	system("pause");
    return 0;
}

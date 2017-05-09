/*
 * UserInterface.cpp
 *
 *  Created on: 8 de mai de 2017
 *      Author: Claus Tessmann
 */

#include "UserInterface.h"
#include <iostream>
#include <thread>

using namespace std;

UserInterface::UserInterface() {
	on = false;
	tela = new thread([this] () {
		while(true) {
			if (this->on){
				this->telaPrincipal();
			}
		}
	});
}

UserInterface::~UserInterface() {
	// TODO Auto-generated destructor stub
}

/**
 * Feito do jeito mais burro pois somos umas topera que n�o sabem fazer UI
 * E o eclipse n�o ajuda nisso
 * E n�o tem conio2.h aqui tamb�m
 * E a conio.h n�o � util para nada
 */
void UserInterface::inicia(void) {
	on = true;
}

void UserInterface::telaPrincipal(void) {
	system("cls");
	cout << "\t\tTITULO BESTA " << endl << endl;
	cout << "Placa Desconectada" << endl; //Nessa linha, quando conectado, vir� tamb�m os estados do resistor, ventoinha e temperatura atual
	cout << "Por favor, escolha uma das opcoes a seguir:" << endl;
	cout << "CODIGO \t�\t O QUE O CODIGO FAZ" << endl;
	cout << "Aqui viria uma mensagem de erro" << endl << endl;	//Normalmente essa linha ser� escondida
	cout << "> ";
}

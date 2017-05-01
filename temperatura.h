#include <cstdio>
#include <iostream>
#include <string>
#include <windows.h>


#ifndef TEMPERATURA_H_
#define TEMPERATURA_H_


using namespace std;
class portaSerial{
HANDLE h;  

public:
  //Abre a porta serial número N
  void abrir(int porta);
  // Encerra uma conexão
  void fechar();
  // Envia um comando pela porta seria e recebe a reposta como String
  string enviaSerial(int n);
};

#endif

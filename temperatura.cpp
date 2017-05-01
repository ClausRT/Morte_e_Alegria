#include <cstdio>
#include <iostream>
#include <string>
#include <windows.h>
#include "serialport.h"
#include "temperatura.h"
using namespace std;

void portaSerial::abrir(int porta){
    string s;
    char ps[2];
    itoa(porta,ps,10);
    s = string("COM")+string(ps);
    h = openSerialPort(s.c_str(),B9600,one,off);
  }
  
void portaSerial::fechar(){
   closeSerialPort(h);
  }
  
string portaSerial::enviaSerial(int n){
    char sendbuffer[2];
	char readbuffer[100];
	itoa(n,sendbuffer,10);
	int bytesWritten = writeToSerialPort(h,sendbuffer,strlen(sendbuffer));
	int bytesRead = readFromSerialPort(h,readbuffer,99);
	printf("%d",bytesRead);
	readbuffer[bytesRead]=0;
	return string(readbuffer);     
  }

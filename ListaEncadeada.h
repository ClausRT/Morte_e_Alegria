#ifndef LISTAENCADEADA_H_
#define LISTAENCADEADA_H_
#include <cstdlib>
#include <iostream>
#include "elemento.h"
#include "elemento.cpp"
using namespace std;

template<typename T>
class ListaEncadeada { //Defini��o da Classe
protected:
  int tam; // Tamanho da lista
  Elemento<T> *cabeca; // A "cabe�a" � um ponteiro para o primeiro elemento da lista.
public:
  ListaEncadeada(); // Construtor padr�o
  void insereF(T x); // M�todo para adicionar um elemento novo ao final da lista.
  void insereI(T x); // M�todo para adicionar um elemento novo no in�cio.
  void insere(T x, int pos); // M�todo para inserir em uma posi��o qualquer
  void imprime();  // M�todo para imprimir, na sa�da padr�o, todos os elementos na tela;
  bool localiza(T val);  // M�todo de busca de um elemento na lista
  bool delU(); // Apaga o �ltimo elemento de uma lista
  bool delP(); // Apaga o primeiro elemento de uma lista
  bool delV(T y); // Apaga o elemento de valor y
  T pos(int pos); // Retorna a posi��o do elemento p;

	int getTam() const {
		return tam;
	}
};
#endif // LISTAENCADEADA_H_

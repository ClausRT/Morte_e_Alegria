#ifndef LISTAENCADEADA_H_
#define LISTAENCADEADA_H_
#include <cstdlib>
#include <iostream>
#include "elemento.h"
#include "elemento.cpp"
using namespace std;

template<typename T>
class ListaEncadeada { //Definição da Classe
protected:
  int tam; // Tamanho da lista
  Elemento<T> *cabeca; // A "cabeça" é um ponteiro para o primeiro elemento da lista.
public:
  ListaEncadeada(); // Construtor padrão
  void insereF(T x); // Método para adicionar um elemento novo ao final da lista.
  void insereI(T x); // Método para adicionar um elemento novo no início.
  void insere(T x, int pos); // Método para inserir em uma posição qualquer
  void imprime();  // Método para imprimir, na saída padrão, todos os elementos na tela;
  bool localiza(T val);  // Método de busca de um elemento na lista
  bool delU(); // Apaga o último elemento de uma lista
  bool delP(); // Apaga o primeiro elemento de uma lista
  bool delV(T y); // Apaga o elemento de valor y
  T pos(int pos); // Retorna a posição do elemento p;

	int getTam() const {
		return tam;
	}
};
#endif // LISTAENCADEADA_H_

#include "ListaEncadeada.h"
#include "any_cast.cpp"

template<typename T>
ListaEncadeada<T>::ListaEncadeada() {
  cabeca = NULL; // Lista vazia
  tam = 0;
}

template<typename T>
void ListaEncadeada<T>::insereF(T x){//Adiciona um elemento ao final da lista
  Elemento<T> *novo = new Elemento<T>(x); // Aloca espaço de memória
  tam++;
  if (!cabeca) // Lista vazia
    cabeca = novo;
  else { // Lista já tem elementos
    Elemento<T> *onde = cabeca;
    while (onde->prox) // Enquanto não for o último elemento
      onde = onde->prox;
    onde->prox = novo;
  }
}

template<typename T>
void ListaEncadeada<T>::insereI(T x) { // Método para adicionar um elemento novo ao final da lista.
  Elemento<T> *novo = new Elemento<T>(x); // Aloca espaço de memória
  tam++;
  novo->prox = cabeca;
  cabeca = novo;
}

template<typename T>
void ListaEncadeada<T>::insere(T x, int pos) {
  if (pos == 0)
    insereI(x);
  else if (pos == tam)
    insereF(x);
  else if ((pos < tam) && (pos > 0)) {
    Elemento<T> *novo = new Elemento<T>(x); // Aloca espaço de memória
    tam++;
    Elemento<T> *onde = cabeca;
    for (int i = 0; i < (pos - 1); ++i) // Avança até a posição de inserção
      onde = onde->prox;
    novo->prox = onde->prox;
    onde->prox = novo;
  }
}

template<class T>
void ListaEncadeada<T>::imprime() { // Método para imprimir, na saída padrão, todos os elementos na tela;
  Elemento<T>* temp = cabeca;
  do
    cout << temp->valor << endl;
  while (temp = temp->prox); // É uma atribuição e uma comparação
}

template<typename T>
bool ListaEncadeada<T>::localiza(T val) { // Método de busca de um elemento na lista
  for (Elemento<T> *onde = cabeca; onde; onde = onde->prox)
    if (onde->valor == val)
      return true;
  return false;
}

template<typename T>
bool ListaEncadeada<T>::delU() {
  if (tam > 0) {
    tam--;
    Elemento<T> *onde = cabeca;
    while (onde->prox->prox) // Enquanto não for o penúltimo elemento
      onde = onde->prox;
    delete onde->prox; // libera a área de memória do ponteiro
    onde->prox = NULL; // define o penultimo elemento como prox NULL.
    return true;
  }
  return false;
}

template<typename T>
bool ListaEncadeada<T>::delP() {
  if (tam > 0) {
    tam--;
    Elemento<T> *lixo = cabeca;
    cabeca = cabeca->prox;
    delete lixo; // libera a área de memória do ponteiro
    return true;
  }
  return false;
}

template<typename T>
bool ListaEncadeada<T>::delV(T y) {
  if ((tam > 0)&&(localiza(y))) {
    Elemento<T> *onde = cabeca;
    if (onde->valor == y)
      return delP();
    tam--;
    while (onde->prox->valor != y) // ve o valor do nó seguinte
      onde = onde->prox; // Avança
    Elemento<T> *lixo = onde->prox;
    onde->prox = onde->prox->prox;
    delete lixo;
    return true;
  }
  return false;
}

/**
 * Professor, alterei o método a baixo pois, como T será um objeto de uma classe Dado, ele não
 * pode retornar -1 caso falhe na verificação de segurança.
 */

template<typename T>
T ListaEncadeada<T>::pos(int pos) {
  if ((pos < tam) && (pos >= 0)) { //Verifica se a pos é valida
    Elemento<T> *onde = cabeca; // Ponteiro auxiliar
    for (int i = 0; i < pos; i++) // Avança até a posição que se queira
      onde = onde->prox; // Avança 1 posição
    return onde->valor; //Retorna o valor
  } else {
	  // TODO O que eu tentei não dei certo. Voltar aqui depois.
  }
}

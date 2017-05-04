#include "ListaEncadeada.h"
#include "any_cast.cpp"

template<typename T>
ListaEncadeada<T>::ListaEncadeada() {
  cabeca = NULL; // Lista vazia
  tam = 0;
}

template<typename T>
void ListaEncadeada<T>::insereF(T x){//Adiciona um elemento ao final da lista
  Elemento<T> *novo = new Elemento<T>(x); // Aloca espa�o de mem�ria
  tam++;
  if (!cabeca) // Lista vazia
    cabeca = novo;
  else { // Lista j� tem elementos
    Elemento<T> *onde = cabeca;
    while (onde->prox) // Enquanto n�o for o �ltimo elemento
      onde = onde->prox;
    onde->prox = novo;
  }
}

template<typename T>
void ListaEncadeada<T>::insereI(T x) { // M�todo para adicionar um elemento novo ao final da lista.
  Elemento<T> *novo = new Elemento<T>(x); // Aloca espa�o de mem�ria
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
    Elemento<T> *novo = new Elemento<T>(x); // Aloca espa�o de mem�ria
    tam++;
    Elemento<T> *onde = cabeca;
    for (int i = 0; i < (pos - 1); ++i) // Avan�a at� a posi��o de inser��o
      onde = onde->prox;
    novo->prox = onde->prox;
    onde->prox = novo;
  }
}

template<class T>
void ListaEncadeada<T>::imprime() { // M�todo para imprimir, na sa�da padr�o, todos os elementos na tela;
  Elemento<T>* temp = cabeca;
  do
    cout << temp->valor << endl;
  while (temp = temp->prox); // � uma atribui��o e uma compara��o
}

template<typename T>
bool ListaEncadeada<T>::localiza(T val) { // M�todo de busca de um elemento na lista
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
    while (onde->prox->prox) // Enquanto n�o for o pen�ltimo elemento
      onde = onde->prox;
    delete onde->prox; // libera a �rea de mem�ria do ponteiro
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
    delete lixo; // libera a �rea de mem�ria do ponteiro
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
    while (onde->prox->valor != y) // ve o valor do n� seguinte
      onde = onde->prox; // Avan�a
    Elemento<T> *lixo = onde->prox;
    onde->prox = onde->prox->prox;
    delete lixo;
    return true;
  }
  return false;
}

/**
 * Professor, alterei o m�todo a baixo pois, como T ser� um objeto de uma classe Dado, ele n�o
 * pode retornar -1 caso falhe na verifica��o de seguran�a.
 */

template<typename T>
T ListaEncadeada<T>::pos(int pos) {
  if ((pos < tam) && (pos >= 0)) { //Verifica se a pos � valida
    Elemento<T> *onde = cabeca; // Ponteiro auxiliar
    for (int i = 0; i < pos; i++) // Avan�a at� a posi��o que se queira
      onde = onde->prox; // Avan�a 1 posi��o
    return onde->valor; //Retorna o valor
  } else {
	  // TODO O que eu tentei n�o dei certo. Voltar aqui depois.
  }
}

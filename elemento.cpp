#include "elemento.h"
// Construtor, Caso um valor n�o seja atributido ser� utilizado zero
template<typename T>
Elemento<T>::Elemento(T v) {
  valor = v;
  prox = NULL;
}

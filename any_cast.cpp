/*
 * any_cast.cpp
 *
 *  Created on: 3 de mai de 2017
 *      Author: Claus Tessmann
 */

/**
 * Tirando de http://stackoverflow.com/questions/5055636/casting-an-object-to-char-for-saving-loading
 * A ideia é converter objetos de um tipo para o outro genéricamente
 * A classe Monitoramento a usa para trabalhar com os buffers de leitura e escrita de arquivos em binário
 */
template<class To, class From>
To any_cast(From v)
{
    return static_cast<To>(static_cast<void*>(v));
}

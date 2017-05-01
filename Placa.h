/*
 * Placa.h
 *
 *  Created on: 4 de abr de 2017
 *      Author: Claus Tessmann
 */

#include <string>
#include <temperatura.h>

#ifndef PLACA_H_
#define PLACA_H_

class Placa {
	bool _estadoResistor;
	bool _estadoVentoinha;
	bool _estadoLampada;
	portaSerial _serial;
public:
	Placa(int porta);
	virtual ~Placa();
	bool resistor(bool acionamento);
	bool ventoinha(bool acionamento);
	bool lampada(bool acionamento);
	string temperatura(void);

	bool isEstadoLampada() const {
		return _estadoLampada;
	}

	bool isEstadoResistor() const {
		return _estadoResistor;
	}

	bool isEstadoVentoinha() const {
		return _estadoVentoinha;
	}
};

#endif /* PLACA_H_ */

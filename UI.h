/*
 * UI.h
 *
 *  Created on: 7 de mai de 2017
 *      Author: Claus Tessmann
 */
#ifndef UI_H_
#define UI_H_
#include <string>

using namespace std;

class UI {
public:
	UI();
	virtual ~UI();
	string valData(string a);
	void subConsulta(void);
	int subSalva(int i);
	int subLimpa(int i);
	int desenha(int i=-1, int t=0);
	void desenhaUI(void);
};
#endif /* UI_H_ */

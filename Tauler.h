#ifndef TAULER_H
#define TAULER_H

#include <iostream>
#include <string.h>
#include "Posicio.h"
#include "Fitxa.h"
#include <fstream>

using namespace std;

class Tauler
{
public:
	void inicialitza(const string& nomFitxer); //inicialitzaem el fitxer segons un document de text
	void actualitzaMovimentsValids();
	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
	int m_fila, m_columna;
};

//void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);

#endif  TAULER_H
#include <iostream>
#include <string.h>
#include "Posicio.h"
#include "Fitxa.h"
using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

class Tauler
{
public:
	void inicialitza(const string& nomFitxer);
	void actualitzaMovimentsValids();
	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

};
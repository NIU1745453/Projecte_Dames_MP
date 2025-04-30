#include <iostream>
#include <string>
#include "Posicio.h"

const int MAX_PECES = 24;

class Moviment
{
public:
	Moviment() { m_posicio = Posicio(); }
	Moviment(string mov) { m_posicio = Posicio(mov); }

	string getMoviment() const { return m_posicio.getPosicio(); }

	void setMoviment(string mov) { m_posicio.setPosicio(mov); }

	bool compatible(Posicio posicio); //mirara si el moviment que mira es pot fer o no,
													// en cas de que es pugui fer el mov, l'haura de guardar per
													//tal de poderlo tenir en l'array moviments possibles

	string camiViable(Posicio& posActual);

private:
	Posicio m_posicio;//tiene variable fila, columna y string
};

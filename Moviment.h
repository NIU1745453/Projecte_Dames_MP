#include <iostream>
#include <string>
#include "Posicio.h"

const int MAX_PECES = 24;

class Moviment
{
public:
	Moviment() {
		for (int i = 0; i < 50; i++)
		{
			m_posicio[i] = Posicio();
		}
		m_nPosicio = 0;
	}

	string getMoviment(int i) const { return m_posicio[i].getPosicio(); }
	int getnMoviment() { return m_nPosicio; }

	void setMoviment(string mov, int i) { m_posicio[i].setPosicio(mov); }
	void setnMoviment(int i) { m_nPosicio = i; }

	string camiViable(Posicio& posActual, Fitxa tauler[N_FILES][N_COLUMNES]);
	bool mouFitxa(const Posicio& origen, const Posicio& desti, Fitxa tauler[N_FILES][N_COLUMNES]);
	/*
	Mou la peça que ocupa la posició del paràmetre origen a la posició del paràmetre desti.
		Ha de comprovar que la posició destí estigui dins dels moviments vàlids de la peça.
		Si no ho està no fa el moviment i retorna false.
	A part de moure la peça de la posició origen a la posició destí ha de fer el següent:
		Eliminar del tauler totes les peces que es matin fent el moviment.
		Si la peça arriba al final del tauler, convertir-la en una dama.
		Si el moviment no ha matat una peça contrària quan hi ha altres moviments que sí permeten
		fer-ho o el moviment no ha matat el màxim de peces contràries possibles, bufar una peça del
		jugador (eliminar la peça del tauler) , seguint els criteris que s’han explicat a les regles del joc.
	*/

private:
	Posicio m_posicio[50];//tiene variable fila, columna y string
	int m_nPosicio;
};

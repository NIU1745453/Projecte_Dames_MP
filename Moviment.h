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
	Mou la pe�a que ocupa la posici� del par�metre origen a la posici� del par�metre desti.
		Ha de comprovar que la posici� dest� estigui dins dels moviments v�lids de la pe�a.
		Si no ho est� no fa el moviment i retorna false.
	A part de moure la pe�a de la posici� origen a la posici� dest� ha de fer el seg�ent:
		Eliminar del tauler totes les peces que es matin fent el moviment.
		Si la pe�a arriba al final del tauler, convertir-la en una dama.
		Si el moviment no ha matat una pe�a contr�ria quan hi ha altres moviments que s� permeten
		fer-ho o el moviment no ha matat el m�xim de peces contr�ries possibles, bufar una pe�a del
		jugador (eliminar la pe�a del tauler) , seguint els criteris que s�han explicat a les regles del joc.
	*/

private:
	Posicio m_posicio[50];//tiene variable fila, columna y string
	int m_nPosicio;
};

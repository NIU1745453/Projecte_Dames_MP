#ifndef TAULER_H
#define TAULER_H

#include "Fitxa.h"
#include "posicio.hpp"
#include <fstream>
#include <vector>

using namespace std;

class Tauler
{
public:
	Tauler()
	{
		m_nPosicions = 0;
		for (int i = 0; i < N_FILES; i++)
		{
			for (int j = 0; j < N_COLUMNES; j++)
			{
				Fitxa fit;
				m_tauler[i][j] = fit;
			}
		}
	}
	void setNPosicions(int nPos) { m_nPosicions = nPos; }
	int getNPosicions() { return m_nPosicions; }
	void inicialitza(const string& nomFitxer); //inicialitzaem el fitxer segons un document de text

	void actualitzaMovimentsValids();//actualiza los movimientos validos de las fitxas del tablero

	void calcularCapturas(const Posicio& origen, const Posicio& desti, int& captures, int& damesCapturades);

	void getPosicionsImmediates(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	//esta funcion solo devuelve las posiciones immediatas desde una ficha de origen, y por tanto sera mejor para la funcion actualitza mov vàlids
	//dado que de esta manera podemos hacer el camino más óptimo

	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	/* El mètode retorna a l’array posicionsPossibles totes les posicions on es podria moure la peça
que ocupa la posició origen, tenint en compte tots els passos de qualsevol moviment vàlid de la
peça. nPosicions indica el nº total de posicions possibles.
Posicions possibles de la posició ‘b2’:[‘a3’,‘d4’,‘f6’,‘d8’,‘h8’]*/
	void buscaCaptures(const Posicio& actual, const Fitxa& fitxa, Moviment movimentActual, vector<Moviment>& resultats);


	bool posicioExistent(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	/*comprova que la posicio que s'ha pasat existeix dins de l'array de posicionsPossibles*/

	//Moviment camiViable(Posicio& posActual);

	bool mouFitxa(const Posicio& origen, const Posicio& desti);
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
	//void eliminaFitxa(const Posicio& origen);
	//elimina la fitxa del tauler

	//void toDama(const Posicio& origen);
	//Comprova que la posicio introduida sigui la correcta per a convertir una fitxa en Dama



	string toString() const;
	/*Genera un string amb l’estat actual del tauler de joc en el format que especifiquem a continuació
	8: _ X _ D _ _ _ X
	7: _ _ _ _ _ _ X _
	6: _ _ _ _ _ _ _ _
	5: R _ _ _ _ _ _ _
	4: _ _ _ _ _ _ _ O
	3: _ _ O _ _ _ O _
	2: _ O _ _ _ _ _ X
	1: O _ O _ D _ _ _
	   A B C D E F G H
	creo q ya se como se hace
	*/

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
	int m_nPosicions;
	Posicio m_posicionsPossibles[MAX_MOVIMENTS];
};

//void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES]);


#endif 
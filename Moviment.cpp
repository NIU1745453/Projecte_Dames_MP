#include "Moviment.h"

string Moviment::camiViable(Posicio& posActual, Fitxa Tauler[N_FILES][N_COLUMNES])
{



	/*movimentsValids = []
		movimentsPendents = []
		movimentActual = []
		afegir movimentActual a movimentsPendents
		posicioActual = posicio de la Fitxa
		do
		{
			movimentActual = primer element de movimentsPendents
				posValides = PosicionsValides(posicioActual)
				while (posValides != [])
				{
					afegir a movimentActual la primera posicio valida
						per la resta de posicions valides
						movimentPendent = duplicaMoviment(movimentActual)
						afegir la posicio a movimentPendent
						afegir movimentPendent a movimentsPendents
						posicioActual = primera posicio valida
						posValides = PosicionsValides(posicioActual)
				}
			if (movimentActual != [])
				afegir movimentActual a movimentsValids
		} while movimentsPendents != []*/
}
bool Moviment::mouFitxa(const Posicio& origen, const Posicio& desti, Fitxa m_tauler[N_FILES][N_COLUMNES])//no habia visto que son tipos posicio
{
	bool fet = false;
	bool penalitzar = false;

	int muertas[12];

	int i = 0;
	int x = 0;
	int j = 0;
	int y = 0;
	while (i < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && fet == false)
	{
		while (x < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() && fet == false)
		{
			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x) == desti)
			{
				fet = true;
			}
			else
			{
				x++;
			}
		}
		if (fet = false)
		{
			i++;
			x = 0;
		}
	}

	if (fet && m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_NORMAL)
	{
		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1 != x)
		{
			penalitzar = true;
		}
		if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() + 1].getPosicio() &&
			m_tauler[origen.getFila() + 1][origen.getColumna() + 1].getPosicio() == TIPUS_EMPTY) ||
			m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getPosicio() &&
			m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getPosicio() == TIPUS_EMPTY) ||
			m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getPosicio() &&
			m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getPosicio() == TIPUS_EMPTY ) ||
			m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getPosicio() &&
			m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getPosicio() == TIPUS_EMPTY))
		{
			while (y < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
			{
				if()
			}
		}
		
		while (j < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
		{
			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment()< m_tauler[origen.getFila()][origen.getColumna()].getMoviments(j).getnMoviment())
			{
				penalitzar = true;
			}
			else
			{
				j++;
			}
		}

	}


	if (fet && m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_DAMA)
	{
		while (j < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
		{
			while (x < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getnMoviment() && penalitzar == false)
			{
				if ()
			}
		}
	}
	return fet;
}
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

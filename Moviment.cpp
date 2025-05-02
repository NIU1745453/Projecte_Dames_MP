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

	int direX;
	int direY;

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
		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1 != x)//no hace el camino completo
		{
			penalitzar = true;
		}
		//mira cuando el movimiento seleccionado se mueve unicamente 1 espacio si ese es el movimiento optimo
		if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() + 1].getPosicio() &&
			m_tauler[origen.getFila() + 1][origen.getColumna() + 1].getTipus() == TIPUS_EMPTY) ||
			m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getPosicio() &&
			m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getTipus() == TIPUS_EMPTY) ||
			m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getPosicio() &&
			m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getTipus() == TIPUS_EMPTY ) ||
			m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getPosicio() &&
			m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getTipus() == TIPUS_EMPTY))
		{
			while (y < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
			{
				if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() + 2][origen.getColumna() + 2].getPosicio() && ||
						m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() - 2][origen.getColumna() - 2].getPosicio() && ||
						m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() - 2][origen.getColumna() + 2].getPosicio() && ||
						m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() + 2][origen.getColumna() - 2].getPosicio() &&)
				{
						penalitzar = true;
				}
				y++;
			}
		}
		else//caso en el que si que hace todo el camino
		{


			while (j < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
			{
				if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(j).getnMoviment())
				{
					penalitzar = true;
				}
				else
				{
					j++;
				}
			}
			
		}

		if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) != 0)
		{
			if (m_tauler[origen.getFila()][origen.getColumna()].getColor() == COLOR_BLANC)// para saber si va hacia arriba o hacia abajo
			{
				direY = 1;
			}
			else
			{
				direY = -1;
			}
			j = 0;

			if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila()>0)//Mira si se mueve hacia la izquierda
			{
				direX = -1
			}
			else//mira si se mueve hacia la derecha
			{
				direX = 1;
			}
			m_tauler[origen.getFila() + direX][origen.getColumna() + direY] = Fitxa();

			for (j = 0; j < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getnMoviment()-1; j++)//para todos los elementos del camino
			{
				if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j+1).getFila() > 0)//Mira si se mueve hacia la izquierda
				{
					direX = -1
				}
				else//mira si se mueve hacia la derecha
				{
					direX = 1;
				}
				m_tauler[origen.getFila() + direX][origen.getColumna() + direY] = Fitxa();
			}
		}


		if (penalitzar = true)//si ha hecho el movimiento optimo lo elimina
		{
			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
		else//si el movimiento es optimo mueve la fitxa
		{
			m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1).getFila()][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1).getColumna()] =
				Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor, m_tauler[origen.getFila()][origen.getColumna()].getFila(), m_tauler[origen.getFila()][origen.getColumna()].getColumna);

			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
	}


	if (fet && m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_DAMA)//lo mismo que el anterior pero para el tipo dama
	{
		while (j < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
		{
			while (x < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getnMoviment() && penalitzar == false)
			{
				if ()
			}
		}

		if (penalitzar = true)
		{
			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
		else
		{
			m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1).getFila()][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1).getColumna()] =
				Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor, m_tauler[origen.getFila()][origen.getColumna()].getFila(), m_tauler[origen.getFila()][origen.getColumna()].getColumna);

			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
	}

	

	return fet;
}
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

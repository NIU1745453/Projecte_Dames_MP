#include "tauler.hpp"

void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);

	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			m_tauler[i][j].setTipus(TIPUS_EMPTY);
		}
	}

	if (fitxer.is_open()) // no se abre el fixero
	{
		string line; //conte le linia completa
		string posFitxa; //conte només la posicio
		char tipusFitxa; //conte el tipus de la fitxa
		Posicio pos;
		int fila;
		int columna;

		while (getline(fitxer, line)) //el fitxer.eof dona sempre bucle inf. per tant hem usat una variable
		{
			if (line != "")
			{
				tipusFitxa = line[0];
				posFitxa = line.substr(2, 2); //trobada de cplusplus.com (serveis per agafar x characters d'un string apartir de y pos)
				pos.fromString(posFitxa);
				fila = pos.getFila();
				columna = pos.getColumna();

				//identifiquem el tipus de la fitxa
				if (tipusFitxa == 'D' || tipusFitxa == 'R')
					m_tauler[fila][columna].setTipus(TIPUS_DAMA);
				else
					m_tauler[fila][columna].setTipus(TIPUS_NORMAL);

				//identigiquem el color de la fitxa
				if (tipusFitxa == 'D' || tipusFitxa == 'O')
					m_tauler[fila][columna].setColor(COLOR_BLANC);
				else
					m_tauler[fila][columna].setColor(COLOR_NEGRE);

			}
		}
		fitxer.close();
	}
}

void Tauler::actualitzaMovimentsValids()
{
	int i = 0;
	int nPos;

	// Arrays de movimientos
	Moviment movPendents[MAX_MOVIMENTS];
	Moviment movimentPendent;
	Posicio posValides[MAX_MOVIMENTS];
	Posicio posicioActual;
	posicioActual = posicioActual.getPosicio();
	Moviment movimentActual;
	Moviment movimentsValids[MAX_MOVIMENTS];

	// Inicializar movimientos vacíos
	for (int y = 0; y < MAX_MOVIMENTS; y++) {
		movPendents[y].setMoviment("", y);
		posValides[y].setPosicio("");
		movimentsValids[y].setMoviment("", y);
	}

	// Crear primer movimiento vacío (sin posición inicial)
	movimentActual.setMoviment("", 0);
	int nPosicions = 0;
	movimentActual.setnMoviment(nPosicions);

	// Añadirlo a pendientes
	movPendents[0] = movimentActual;
	int nPendents = 1;
	int nValids = 0;

	// Bucle principal para procesar movimientos pendientes
	while (nPendents > 0) {
		// Tomar el primer movimiento pendiente
		movimentActual = movPendents[0];

		// Desplazar el resto de los movimientos pendientes hacia adelante
		for (int j = 0; j < nPendents - 1; j++) {
			movPendents[j] = movPendents[j + 1];
		}
		nPendents--;

		// Obtener posiciones posibles desde la posición actual
		getPosicionsPossibles(posicioActual, nPos, posValides);

		if (nPos > 0) {
			// Añadir primera posición válida al movimiento actual
			movimentActual.setMoviment(posValides[0].getPosicio(), movimentActual.getnMoviment());
			nPosicions++;
			movimentActual.setnMoviment(nPosicions);

			// Para las demás posiciones válidas
			for (int k = 1; k < nPos; k++) {
				movimentPendent = movimentActual;
				movimentPendent.setMoviment(posValides[k].getPosicio(), (movimentActual.getnMoviment() - 1));
				// Añadir el movimiento pendiente a la lista si no se excede el límite
				if (nPendents < MAX_MOVIMENTS)
					movPendents[nPendents++] = movimentPendent;
			}

			// Volver a poner el movimiento actual en pendientes, si no excede el límite
			if (nPendents < MAX_MOVIMENTS)
				movPendents[nPendents++] = movimentActual;
		}
		else {
			// Si no hay posiciones válidas y el movimiento tiene más de una posición
			if (movimentActual.getnMoviment() > 1) {
				if (nValids < MAX_MOVIMENTS)
					movimentsValids[nValids++] = movimentActual;
			}
		}
	}
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	bool moviment = true;
	nPosicions = 0;
	int pos = 0;
	Posicio newPos = origen;
	int columna = newPos.getColumna();
	int fila = newPos.getFila();
	ColorFitxa colorOrig = m_tauler[fila][columna].getColor(); //agafa el color de la de fitxa per saber de quin equip es
	Posicio perVisitar[MAX_MOVIMENTS]; //guarda les posicions que contenen una fitxa de color contrari
	int Visitar = 0, bucle = 0;
	bool trobat = false;
	
	//(newPos.getColumna() <= 7 && newPos.getColumna() >= 0) && (newPos.getFila() <= 7 && newPos.getFila() >= 0)
	while (pos < MAX_MOVIMENTS && trobat == false)
	{
		for (int i = (newPos.getFila() -1); i < N_FILES; i += 2) //mira las esquinas
		{
			for (int j = (newPos.getColumna() -1); i < N_COLUMNES; i += 2)
			{
				if (i < 8 && i> 0 && j < 8 && j > 0) //nomes actua si esta dins dels parametres per tal de no entrar a pos inexistents de l'array
				{
					newPos.setPosicio(i, j); // guarda la posicion en newPos
					if (!posicioExistent(newPos, nPosicions, posicionsPossibles)) //mira que no este ya añadida en el array (para no repatir)
					{
						columna = perVisitar[pos].getColumna();
						fila = perVisitar[pos].getFila();
						if(m_tauler[i][j].getTipus() == TIPUS_EMPTY)
							posicionsPossibles[nPosicions++] = newPos; //si es una posicion bacia la añade sin mas
						else if (m_tauler[i][j].getColor() != colorOrig && m_tauler[fila][columna].getColor() != m_tauler[i][j].getColor()) //si es de un color diferente y la anterior es del color de la fitxa en juego la mete en el array para visitar
						{
							perVisitar[Visitar] = newPos;
							Visitar++;
						}
					}
				}
			}
		}
		pos++; //suma uno para hacer otra vuelta en el bucle

		if (Visitar != 0)
		{
			newPos = perVisitar[bucle]; //edita newPos para visitar la pos con una fitxa de color dif
			bucle++;
		}
		else
			trobat = true;
	}
	setNPosicions(nPosicions);
}

bool Tauler::posicioExistent(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	bool trobat = false;
	for (int i = 0; i < nPosicions; i++)
	{
		if (posicionsPossibles[i] == origen)
		{
			trobat = true;
		}
	}
	return trobat;
}

//se tiene que repasar pq no estoy segura de que funcione bien del todo
//Moviment Tauler::camiViable(Posicio& posActual)
//{
//	//ALGORISME PER MIRAR ELS MOVIMENTS QUE ES PODEN FER
//	int i = 0, x = 1;
//	int nPos, posVal;
//
//	Moviment movPendents[MAX_MOVIMENTS], movimentPendent[MAX_MOVIMENTS];
//	Moviment posValides[MAX_MOVIMENTS];
//	Moviment movimentActual[MAX_MOVIMENTS];
//	movPendents[0] = movimentActual;
//	Posicio posicioActual = posActual;
//
//	for (int y = 0; y < MAX_MOVIMENTS; y++)
//	{
//		movPendents[y].setMoviment('', y);
//		movimentPendent[y].setMoviment('', y);
//		posValides[y].setMoviment('', y);
//		movimentActual[y].setMoviment('', y);
//
//	}
//
//	Moviment movValids[MAX_MOVIMENTS]; //se le tendra q cambiar el nombre para adaptarlo a la actualizacion de moviment.h
//
//	do
//	{
//		movimentActual[0] = movPendents[i];
//		getPosicionsPossibles(posicioActual, nPos, posValides);
//
//		while (posValides != [] && x < MAX_MOVIMENTS)
//		{
//			movimentPendent = movimentActual;
//			movimentActual[x] = posValides[0];
//			for (int j = 0; j < nPos; j++)
//			{
//				movimentPendent[x] = posValides[j + 1];
//				movPendents[j] = movimentPendent;
//			}
//			posicioActual = movimentActual[x];
//			getPosicionsPossibles(posicioActual, nPos, posValides);
//			x++
//		}
//		x = 0;
//		if (movimentActual != [])
//			movValids[i] = movimentActual;
//
//		i++;
//
//	} while (movimentsPendents != [] && i < MAX_MOVIMENTS);
//
//	return movValids;
//
//
//	/*movimentsValids = []
//		movimentsPendents = []
//		movimentActual = []
//		afegir movimentActual a movimentsPendents
//		posicioActual = posicio de la Fitxa
//		do
//		{
//			movimentActual = primer element de movimentsPendents
//			posValides = PosicionsValides(posicioActual)
//			while (posValides != [])
//			{
//			afegir a movimentActual la primera posicio valida
//				per la resta de posicions valides
//				movimentPendent = duplicaMoviment(movimentActual)
//				afegir la posicio a movimentPendent
//				afegir movimentPendent a movimentsPendents
//				posicioActual = primera posicio valida
//				posValides = PosicionsValides(posicioActual)
//			}
//			if (movimentActual != [])
//				afegir movimentActual a movimentsValids
//		} while movimentsPendents != []*/
//
//}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	bool fet = false;//si se puede hacer el movimiento
	bool penalitzar = false;//si hay que bufarla

	int muerta1;
	int distancia;

	int muerta2;

	int i = 0;
	int x = 0;
	int j = 0;
	int y = 0;

	int direX;
	int direY;

	while (i < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && fet == false)//Mira si la posicion de salida se encuentra en algun camino de la poscion inicial
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
			(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getPosicio() &&
				m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getTipus() == TIPUS_EMPTY) ||
			(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getPosicio() &&
				m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getTipus() == TIPUS_EMPTY) ||
			(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getPosicio() &&
				m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getTipus() == TIPUS_EMPTY))
		{
			while (y < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
			{
				if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() + 2][origen.getColumna() + 2].getPosicio()  || 
					m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() - 2][origen.getColumna() - 2].getPosicio()  ||
					m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() - 2][origen.getColumna() + 2].getPosicio()  ||
					m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() + 2][origen.getColumna() - 2].getPosicio()))
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

		if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() == 2 || m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() == -2)
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

			if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() > 0)//Mira si se mueve hacia la izquierda
			{
				direX = -1;
			}
			else//mira si se mueve hacia la derecha
			{
				direX = 1;
			}
			m_tauler[origen.getFila() + direX][origen.getColumna() + direY] = Fitxa();

			for (j = 0; j < x-1; j++)//para todos los elementos del camino
			{
				if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getFila() > 0)//Mira si se mueve hacia la izquierda
				{
					direX = -1;
				}
				else//mira si se mueve hacia la derecha
				{
					direX = 1;
				}
				m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() + direX][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() + direY] = Fitxa();
			}
		}

		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna() == 0 && direY = -1)
		{
			m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_DAMA);
		}
		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna() == 7 && direY = 1)
		{
			m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_DAMA);
		}

		if (penalitzar = true)//si ha hecho el movimiento optimo lo elimina
		{
			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
		else//si el movimiento es optimo mueve la fitxa
		{
			m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getFila()][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna()] =
				Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor(), m_tauler[origen.getFila()][origen.getColumna()].getPosHorit(), m_tauler[origen.getFila()][origen.getColumna()].getPosVert());

			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
	}


	if (fet && m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_DAMA)//lo mismo que el anterior pero para el tipo dama
	{
		if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() > 0)//Mira si se mueve hacia la izquierda
		{
			direX = -1;
		}
		else//mira si se mueve hacia la derecha
		{
			direX = 1;
		}
		if (m_tauler[origen.getFila()][origen.getColumna()].getPosVert() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() > 0)//abajo
		{
			direY = -1;
		}
		else//arriba
		{
			direY = 1;
		}
		distancia = m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila();

		if (distancia == 2 || distancia == -2)
		{
			muerta1++;
			m_tauler[origen.getFila() + direX][origen.getColumna() + direY] = Fitxa();

		}

		for (j = 0; j < x - 1; j++)
		{
			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getFila() > 0)//Mira si se mueve hacia la izquierda
			{
				direX = -1;
			}
			else//mira si se mueve hacia la derecha
			{
				direX = 1;
			}
			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getColumna() > 0)//Abajo
			{
				direY = -1;
			}
			else//arriba
			{
				direY = 1;
			}
			distancia = m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getFila();


			if (distancia == 2 || distancia == -2)
			{
				muerta1++;
				m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() + direX][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() + direY] = Fitxa();
			}
		}


		while (y < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)//mira si se ha elegido el camino optimo
		{
			if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila() > 0)//Mira si se mueve hacia la izquierda
			{
				direX = -1;
			}
			else//mira si se mueve hacia la derecha
			{
				direX = 1;
			}
			if (m_tauler[origen.getFila()][origen.getColumna()].getPosVert() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getColumna() > 0)//abajo
			{
				direY = -1;
			}
			else//arriba
			{
				direY = 1;
			}
			distancia = m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila();

			if (distancia == 2 || distancia == -2)
			{
				muerta2++;

			}
			while (j < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getnMoviment() - 1 && penalitzar == false)
			{
				if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j + 1).getFila() > 0)//Mira si se mueve hacia la izquierda
				{
					direX = -1;
				}
				else//mira si se mueve hacia la derecha
				{
					direX = 1;
				}
				if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getColumna() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j + 1).getColumna() > 0)//Abajo
				{
					direY = -1;
				}
				else//arriba
				{
					direY = 1;
				}
				distancia = m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j + 1).getFila();


				if (distancia == 2 || distancia == -2)
				{
					muerta1++;
				}
				if (muerta2 > muerta1)
				{
					penalitzar = true;
				}
				j++;
			}
			j = 0;
			muerta2 = 0;
			y++;
		}


		if (penalitzar = true)
		{
			m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
		}
		else
		{
			m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getFila()][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna()] =
				Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor(), m_tauler[origen.getFila()][origen.getColumna()].getPosHorit(), m_tauler[origen.getFila()][origen.getColumna()].getPosHorit());

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

void Tauler::toString(const string& nomFitxer, const Posicio& posicio)
{
	/*
	ofstream fitxer(nomFitxer);

	for (int i = 0; i > N_COLUMNES; i--)
	{
		for(int j = 0; j< N_COLUMNES; j++)
		{
			fitxer << fil << ":" << ' ';
			if (m_tauler[i][j].getTipus() == TIPUS_DAMA)
			{
				if (m_tauler[i][j].getColor() == COLOR_BLANC)
					fitxer << 'D' << ' ';
				else
					fitxer << 'R' << ' ';
			} 
			else if (m_tauler[i][j].getTipus() == TIPUS_NORMAL)
			{
				if (m_tauler[i][j].getColor() == COLOR_BLANC)
					fitxer << 'O' << ' ';
				else
					fitxer << 'X' << ' ';
			}
			else
				fitxer << "_" << ' ';

		}
	}
	

	fitxer.close();
	*/
	int fil = N_FILES;
	for (int i = 0; i > N_COLUMNES; i++)
	{
		cout << fil << ":" << ' ';
		for (int j = 0; j < N_COLUMNES; j++)
		{
			if (m_tauler[i][j].getTipus() == TIPUS_DAMA)
			{
				if (m_tauler[i][j].getColor() == COLOR_BLANC)
					cout << 'D' << ' ';
				else
					cout << 'R' << ' ';
			}
			else if (m_tauler[i][j].getTipus() == TIPUS_NORMAL)
			{
				if (m_tauler[i][j].getColor() == COLOR_BLANC)
					cout << 'O' << ' ';
				else
					cout << 'X' << ' ';
			}
			else
				cout << "_" << ' ';

		}
		fil--;
	}
}

//void EscriuTauler

//void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
//{
//	ofstream fitxer;
//	fitxer.open(nomFitxer);
//
//	if (fitxer.is_open())
//	{
//		char tipFitxa; //conte el tipus de la fitxa
//		//Posicio pos;
//
//		for (int i = N_FILES - 1; i >= 0; i--)
//		{
//			for (int j = 0; j < N_COLUMNES; j++)
//			{
//				if (tauler[i][j] != ' ')
//				{
//					tipFitxa = tauler[i][j];
//					Posicio pos(i, j);
//					fitxer << tipFitxa << ' ';
//					fitxer << pos << "\n";
//				}
//			}
//		}
//
//		fitxer.close();
//	}
//}

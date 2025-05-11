#include "tauler.hpp"
//#include "posicio.hpp"
void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);

	if (fitxer.is_open())
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
				TipusFitxa type;
				ColorFitxa color;

				if(fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES)
				{
					//identifiquem el tipus de la fitxa
					if (tipusFitxa == 'D' || tipusFitxa == 'R')
						type = TIPUS_DAMA;
					else
						type = TIPUS_NORMAL;

					//identigiquem el color de la fitxa
					if (tipusFitxa == 'D' || tipusFitxa == 'O')
						color = COLOR_BLANC;
					else
						color = COLOR_NEGRE;

					m_tauler[fila][columna].setFitxa(type, color, columna, fila);
				}

			}
		}
		fitxer.close();
	}
}

void Tauler::actualitzaMovimentsValids()
{
	int nPos;
	Moviment movPendents[MAX_MOVIMENTS];
	Moviment movimentPendent;
	Posicio posValides[MAX_MOVIMENTS];
	Moviment movimentActual;
	Moviment movimentsValids[MAX_MOVIMENTS];
	int nPendents = 0;
	int nValids = 0;

	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int col = 0; col < N_COLUMNES; col++)
		{
			if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY)
			{
				Posicio posInicial;
				posInicial.setPosicio(fila, col); 

				movimentActual.setMoviment(posInicial.getPosicio(), 0);
				movimentActual.setnMoviment(1);

				movPendents[nPendents++] = movimentActual;

				while (nPendents > 0)
				{
					// Tomamos el primer movimiento pendiente
					movimentActual = movPendents[0];

					// Desplazamos los movimientos pendientes hacia adelante
					for (int j = 0; j < nPendents - 1; j++) {
						movPendents[j] = movPendents[j + 1];
					}
					nPendents--; // Reducimos el número de pendientes

					// Obtenemos la última posición del movimiento actual
					int ultPos = movimentActual.getnMoviment() - 1;
					Posicio posicioActual = movimentActual.getMoviment(ultPos);

					// Obtenemos las posiciones posibles para este movimiento
					getPosicionsPossibles(posicioActual, nPos, posValides);

					if (nPos > 0)
					{
						// Para cada posición válida, creamos un nuevo movimiento
						for (int i = 0; i < nPos; i++)
						{
							Moviment nouMoviment = movimentActual;
							int nMov = nouMoviment.getnMoviment();
							nouMoviment.setMoviment(posValides[i].getPosicio(), nMov);
							nouMoviment.setnMoviment(nMov + 1);

							// Añadimos el nuevo movimiento pendiente si no excedemos el límite
							if (nPendents < MAX_MOVIMENTS)
								movPendents[nPendents++] = nouMoviment;
						}
					}
					else
					{
						// Si el movimiento tiene más de una posición, lo añadimos a los movimientos válidos
						if (movimentActual.getnMoviment() > 1)
						{
							if (nValids < MAX_MOVIMENTS)
								movimentsValids[nValids++] = movimentActual;
						}
					}
				}
			}
		}
	}

	// Finalmente, actualizamos el número de posiciones válidas
	setNPosicions(nValids);
}

//falta TIPUS_DAMA
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	int fila = origen.getFila();
	int columna = origen.getColumna();
	nPosicions = 0;
	int posValides = 1;
	int uno = 1;
	TipusFitxa tipus = m_tauler[fila][columna].getTipus();
	ColorFitxa color = m_tauler[fila][columna].getColor();
	/*
	El codi pot ser simplificat a base d'agefir funcions 
	Blan i negre son igual nomes canvia si puja o baixa del tauler (SOLO CAMBIA FILA)
	*/

	switch (tipus)
	{
	case TIPUS_NORMAL:
		//me podria deshacer de este case interno y haces el codigo mucho mas corto
		switch (color)
		{
		case COLOR_BLANC:
			if ((fila > 0 && fila < 8) && (columna > 0 && columna < 8))
			{
				switch (columna)
				{
				case 0:
				case 7:
					
					if (columna == 7)
						uno = -1;
					if (m_tauler[fila - 1][columna + uno].getColor() == CAP_COLOR)
					{
						posicionsPossibles[nPosicions] = m_tauler[fila - 1][columna + 1].getPosicio();
						nPosicions++;
					}
					if (m_tauler[fila - 1][columna + uno].getColor() == COLOR_NEGRE)
					{
						uno = uno * 2;
						if (m_tauler[fila - 2][columna].getColor() == CAP_COLOR)
						{
							posicionsPossibles[nPosicions] = m_tauler[fila - 2][columna ].getPosicio();
							nPosicions++;
						}
						if (m_tauler[fila - 2][columna + uno].getColor() == CAP_COLOR)
						{
							posicionsPossibles[nPosicions] = m_tauler[fila - 2][columna + uno].getPosicio();
							nPosicions++;
						}
					}
					break;
				case 1 || 6:
					int newFila = fila-1;
					int newColumna = columna + 1;
					//mirem si son buides
					ColorFitxa newColor = m_tauler[newFila][newColumna].getColor();

					if (newColor == CAP_COLOR)
					{
						posicionsPossibles[nPosicions] = m_tauler[newFila][newColumna].getPosicio();
						nPosicions++;
					}
					else if (newColor == COLOR_NEGRE)
					{
						newFila--;
						if (columna != 6)
						{
							if (m_tauler[newFila][columna].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna].getPosicio();
								nPosicions++;
							}
							if (m_tauler[newFila][columna + 2].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna - 2].getPosicio();
								nPosicions++;
							}
						}
					}

					newColor = m_tauler[newFila][columna - 1].getColor();
					newColumna = columna - 1;
					if (newColor == CAP_COLOR)
					{
						posicionsPossibles[nPosicions] = m_tauler[newFila][newColumna].getPosicio();
						nPosicions++;
					}
					else if (newColor == COLOR_NEGRE)
					{
						newFila--;
						if (columna != 1)
						{
							if (m_tauler[newFila][columna].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna].getPosicio();
								nPosicions++;
							}
							if (m_tauler[newFila][columna + 2].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna - 2].getPosicio();
								nPosicions++;
							}
						}
					}
					break;
				}
			}
			break;
		case COLOR_NEGRE:
			if ((fila > 0 && fila < 8) && (columna > 0 && columna < 8))
			{
				switch (columna)
				{
				case 0:
				case 7:
					if (columna == 7)
						uno = -1;
					if (m_tauler[fila - 1][columna + uno].getColor() == CAP_COLOR)
					{
						posicionsPossibles[nPosicions] = m_tauler[fila - 1][columna + 1].getPosicio();
						nPosicions++;
					}
					if (m_tauler[fila - 1][columna + uno].getColor() == COLOR_BLANC)
					{
						uno = uno * 2;
						if (m_tauler[fila - 2][columna].getColor() == CAP_COLOR)
						{
							posicionsPossibles[nPosicions] = m_tauler[fila - 2][columna].getPosicio();
							nPosicions++;
						}
						if (m_tauler[fila - 2][columna + uno].getColor() == CAP_COLOR)
						{
							posicionsPossibles[nPosicions] = m_tauler[fila - 2][columna + uno].getPosicio();
							nPosicions++;
						}
					}
					break;
				case 1 || 6:
					int newFila = fila + 1;
					int newColumna = columna + 1;
					//mirem si son buides
					ColorFitxa newColor = m_tauler[newFila][newColumna].getColor();

					if (newColor == CAP_COLOR)
					{
						posicionsPossibles[nPosicions] = m_tauler[newFila][newColumna].getPosicio();
						nPosicions++;
					}
					else if (newColor == COLOR_BLANC)
					{
						newFila++;
						if (columna != 6)
						{
							if (m_tauler[newFila][columna].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna].getPosicio();
								nPosicions++;
							}
							if (m_tauler[newFila][columna + 2].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna - 2].getPosicio();
								nPosicions++;
							}
						}
					}

					newColor = m_tauler[newFila][columna - 1].getColor();
					newColumna = columna - 1;
					if (newColor == CAP_COLOR)
					{
						posicionsPossibles[nPosicions] = m_tauler[newFila][newColumna].getPosicio();
						nPosicions++;
					}
					else if (newColor == COLOR_BLANC)
					{
						newFila++;
						if (columna != 1)
						{
							if (m_tauler[newFila][columna].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna].getPosicio();
								nPosicions++;
							}
							if (m_tauler[newFila][columna + 2].getColor() == CAP_COLOR)
							{
								posicionsPossibles[nPosicions] = m_tauler[newFila][columna - 2].getPosicio();
								nPosicions++;
							}
						}
					}
					break;
				}
			}
			break;
		}
		break;
	case TIPUS_DAMA:
		break;
	}
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

//bufar per no fer el millor mov en general
//meja mes fitxes, mes dames, mes punts

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	//bool fet = false;//si se puede hacer el movimiento
	//bool penalitzar = false;//si hay que bufarla

	//int muerta1=0;
	//int distancia;

	//int muerta2=0;

	//int i = 0;
	//int x = 0;
	//int j = 0;
	//int y = 0;

	//int direX;
	//int direY;

	//while (i < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && fet == false)//Mira si la posicion de salida se encuentra en algun camino de la poscion inicial
	//{
	//	while (x < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() && fet == false)
	//	{
	//		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x) == desti)
	//		{
	//			fet = true;
	//		}
	//		else
	//		{
	//			x++;
	//		}
	//	}
	//	if (fet = false)
	//	{
	//		i++;
	//		x = 0;
	//	}
	//}

	//if (fet && m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_NORMAL)
	//{
	//	if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() - 1 != x)//no hace el camino completo
	//	{
	//		penalitzar = true;
	//	}
	//	//mira cuando el movimiento seleccionado se mueve unicamente 1 espacio si ese es el movimiento optimo
	//	if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() + 1].getPosicio() &&
	//		m_tauler[origen.getFila() + 1][origen.getColumna() + 1].getTipus() == TIPUS_EMPTY) ||
	//		(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getPosicio() &&
	//			m_tauler[origen.getFila() - 1][origen.getColumna() - 1].getTipus() == TIPUS_EMPTY) ||
	//		(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getPosicio() &&
	//			m_tauler[origen.getFila() - 1][origen.getColumna() + 1].getTipus() == TIPUS_EMPTY) ||
	//		(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j) == m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getPosicio() &&
	//			m_tauler[origen.getFila() + 1][origen.getColumna() - 1].getTipus() == TIPUS_EMPTY))
	//	{
	//		while (y < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
	//		{
	//			if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() + 2][origen.getColumna() + 2].getPosicio()  || 
	//				m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() - 2][origen.getColumna() - 2].getPosicio()  ||
	//				m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() - 2][origen.getColumna() + 2].getPosicio()  ||
	//				m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j) == m_tauler[origen.getFila() + 2][origen.getColumna() - 2].getPosicio()))
	//			{
	//				penalitzar = true;
	//			}
	//			y++;
	//		}
	//	}
	//	else//caso en el que si que hace todo el camino
	//	{


	//		while (j < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)
	//		{
	//			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getnMoviment() < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(j).getnMoviment())
	//			{
	//				penalitzar = true;
	//			}
	//			else
	//			{
	//				j++;
	//			}
	//		}

	//	}

	//	if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() == 2 || m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() == -2)
	//	{
	//		if (m_tauler[origen.getFila()][origen.getColumna()].getColor() == COLOR_BLANC)// para saber si va hacia arriba o hacia abajo
	//		{
	//			direY = 1;
	//		}
	//		else
	//		{
	//			direY = -1;
	//		}
	//		j = 0;

	//		if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() > 0)//Mira si se mueve hacia la izquierda
	//		{
	//			direX = -1;
	//		}
	//		else//mira si se mueve hacia la derecha
	//		{
	//			direX = 1;
	//		}
	//		m_tauler[origen.getFila() + direX][origen.getColumna() + direY] = Fitxa();

	//		for (j = 0; j < x-1; j++)//para todos los elementos del camino
	//		{
	//			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getFila() > 0)//Mira si se mueve hacia la izquierda
	//			{
	//				direX = -1;
	//			}
	//			else//mira si se mueve hacia la derecha
	//			{
	//				direX = 1;
	//			}
	//			m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() + direX][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() + direY] = Fitxa();
	//		}
	//	}

	//	if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna() == 0 && direY == -1)
	//	{
	//		m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_DAMA);
	//	}
	//	if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna() == 7 && direY == 1)
	//	{
	//		m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_DAMA);
	//	}

	//	if (penalitzar = true)//si ha hecho el movimiento optimo lo elimina
	//	{
	//		m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
	//	}
	//	else//si el movimiento es optimo mueve la fitxa
	//	{
	//		m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getFila()][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna()] =
	//			Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor(), m_tauler[origen.getFila()][origen.getColumna()].getPosHorit(), m_tauler[origen.getFila()][origen.getColumna()].getPosVert());

	//		m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
	//	}
	//}


	//if (fet && m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_DAMA)//lo mismo que el anterior pero para el tipo dama
	//{
	//	if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() > 0)//Mira si se mueve hacia la izquierda
	//	{
	//		direX = -1;
	//	}
	//	else//mira si se mueve hacia la derecha
	//	{
	//		direX = 1;
	//	}
	//	if (m_tauler[origen.getFila()][origen.getColumna()].getPosVert() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() > 0)//abajo
	//	{
	//		direY = -1;
	//	}
	//	else//arriba
	//	{
	//		direY = 1;
	//	}
	//	distancia = m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila();

	//	if (distancia == 2 || distancia == -2)
	//	{
	//		muerta1++;
	//		m_tauler[origen.getFila() + direX][origen.getColumna() + direY] = Fitxa();

	//	}

	//	for (j = 0; j < x - 1; j++)
	//	{
	//		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getFila() > 0)//Mira si se mueve hacia la izquierda
	//		{
	//			direX = -1;
	//		}
	//		else//mira si se mueve hacia la derecha
	//		{
	//			direX = 1;
	//		}
	//		if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getColumna() > 0)//Abajo
	//		{
	//			direY = -1;
	//		}
	//		else//arriba
	//		{
	//			direY = 1;
	//		}
	//		distancia = m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j + 1).getFila();


	//		if (distancia == 2 || distancia == -2)
	//		{
	//			muerta1++;
	//			m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getFila() + direX][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(j).getColumna() + direY] = Fitxa();
	//		}
	//	}


	//	while (y < m_tauler[origen.getFila()][origen.getColumna()].getnValidas() && penalitzar == false)//mira si se ha elegido el camino optimo
	//	{
	//		if (m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila() > 0)//Mira si se mueve hacia la izquierda
	//		{
	//			direX = -1;
	//		}
	//		else//mira si se mueve hacia la derecha
	//		{
	//			direX = 1;
	//		}
	//		if (m_tauler[origen.getFila()][origen.getColumna()].getPosVert() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getColumna() > 0)//abajo
	//		{
	//			direY = -1;
	//		}
	//		else//arriba
	//		{
	//			direY = 1;
	//		}
	//		distancia = m_tauler[origen.getFila()][origen.getColumna()].getPosHorit() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila();

	//		if (distancia == 2 || distancia == -2)
	//		{
	//			muerta2++;

	//		}
	//		while (j < m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getnMoviment() - 1 && penalitzar == false)
	//		{
	//			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j + 1).getFila() > 0)//Mira si se mueve hacia la izquierda
	//			{
	//				direX = -1;
	//			}
	//			else//mira si se mueve hacia la derecha
	//			{
	//				direX = 1;
	//			}
	//			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getColumna() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j + 1).getColumna() > 0)//Abajo
	//			{
	//				direY = -1;
	//			}
	//			else//arriba
	//			{
	//				direY = 1;
	//			}
	//			distancia = m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j).getFila() - m_tauler[origen.getFila()][origen.getColumna()].getMoviments(y).getMoviment(j + 1).getFila();


	//			if (distancia == 2 || distancia == -2)
	//			{
	//				muerta1++;
	//			}
	//			if (muerta2 > muerta1)
	//			{
	//				penalitzar = true;
	//			}
	//			j++;
	//		}
	//		j = 0;
	//		muerta2 = 0;
	//		y++;
	//	}


	//	if (penalitzar = true)
	//	{
	//		m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
	//	}
	//	else
	//	{
	//		m_tauler[m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getFila()][m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment(x).getColumna()] =
	//			Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor(), m_tauler[origen.getFila()][origen.getColumna()].getPosHorit(), m_tauler[origen.getFila()][origen.getColumna()].getPosHorit());

	//		m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
	//	}
	//}

	//return fet;
	return false;
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

string Tauler::toString() const
{
	string tauler[N_FILES +1][N_COLUMNES+1];
	ColorFitxa colTauler;
	string sortida = "";
	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			colTauler = m_tauler[i][j].getColor();
			switch (colTauler)
			{
			case COLOR_BLANC:
				if (m_tauler[i][j].getTipus() == TIPUS_DAMA)
					tauler[i][j] = 'D';
				else
					tauler[i][j] = 'O';
				break;
			case CAP_COLOR:
				tauler[i][j] = '_';
				break;
			case COLOR_NEGRE:
				if (m_tauler[i][j].getTipus() == TIPUS_DAMA)
					tauler[i][j] = 'R';
				else
					tauler[i][j] = 'X';
				break;
			}
		}
	}
	string pos;
	for (int i = 0; i < N_FILES; i++)
	{
		
		pos = '8' - i;

		sortida += pos + " :";
		for (int j = 0; j < N_COLUMNES; j++)
		{
			sortida += tauler[i][j];
			sortida += " ";
		}
		sortida += "\n";
	}

	for(int i = 0; i < N_COLUMNES ; i++)
	{
		pos = 'a' + i;
		sortida += " " + pos;
	}

	return sortida;
}

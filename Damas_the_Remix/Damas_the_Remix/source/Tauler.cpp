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
	for (int fila = 0; fila < N_FILES; fila++)
	{
		for (int col = 0; col < N_COLUMNES; col++)
		{
			Moviment movValids[MAX_MOVIMENTS];
			Posicio posInicial(fila, col);
			int nValids = 0;

			Moviment movPendents[MAX_MOVIMENTS];
			int nPendents = 0;

			if (m_tauler[fila][col].getTipus() != TIPUS_EMPTY)
			{
				Moviment movActual;
				movActual.setMoviment(posInicial.toString());
				movActual.setnMoviment(1);
				movPendents[nPendents++] = movActual;

				do
				{
					movActual = movPendents[0];
					for (int j = 0; j < nPendents - 1; j++)
						movPendents[j] = movPendents[j + 1];
					nPendents--;

					int ultPos = movActual.getnMoviment() - 1;
					Posicio posicioActual = movActual.getMoviment();

					Posicio posicionsPossibles[MAX_MOVIMENTS];
					int nPos;
					getPosicionsPossibles(posicioActual, nPos, posicionsPossibles);

					if (nPos > 0)
					{
						for (int i = 0; i < nPos; i++)
						{
							Moviment nouMoviment = movActual;
							int nMov = nouMoviment.getnMoviment();
							nouMoviment.setMoviment(posicionsPossibles[i].toString());
							nouMoviment.setnMoviment(nMov + 1);

							if (nPendents < MAX_MOVIMENTS)
								movPendents[nPendents++] = nouMoviment;
						}
					}
					//else
					{
						if (movActual.getnMoviment() > 1 && nValids < MAX_MOVIMENTS)
							movValids[nValids++] = movActual;
					}

				} while (nPendents > 0);
				m_tauler[fila][col].setMovimentsValids(movValids, nValids);
			}
		}
	}
}

//falta TIPUS_DAMA
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	int fila = origen.getFila();
	int col = origen.getColumna();
	nPosicions = 0;
	int posValides = 1;
	int uno = 1;
	int nPendents = 0;
	int newFila = fila;
	int newColumna = col;
	TipusFitxa tipus = m_tauler[fila][col].getTipus();
	ColorFitxa color = m_tauler[fila][col].getColor();
	int dx[2];
	int dy[2];
	Posicio posPend[3];
	//miramos las direciones que puede mirar la fitxa
	nPosicions = 0;
	Fitxa fitxa = m_tauler[fila][col];

	// desplazamientos diagonales
	int movs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

	if (fitxa.getTipus() == TIPUS_NORMAL) {
		int dir;
		if (fitxa.getColor() == COLOR_BLANC)
			dir = -1;
		else
			dir = 1;

		for (int i = 0; i < 2; i++) {
			int newFila = fila + dir;
			int newCol = col + movs[i][1];

			if (newFila >= 0 && newFila < 8 && newCol >= 0 && newCol < 8) {
				Fitxa dest = m_tauler[newFila][newCol];
				// Si está vacío, es posible
				if (dest.getTipus() == TIPUS_EMPTY) {
					posicionsPossibles[nPosicions++] = Posicio(newFila, newCol);
				}
				// Si hay ficha contraria y detrás está vacío → posible salto
				else if (dest.getColor() != fitxa.getColor() && dest.getTipus() != TIPUS_EMPTY) {
					int saltoFila = newFila + dir;
					int saltoCol = newCol + movs[i][1];
					if (saltoFila >= 0 && saltoFila < 8 && saltoCol >= 0 && saltoCol < 8
						&& m_tauler[saltoFila][saltoCol].getTipus() == TIPUS_EMPTY) {
						posicionsPossibles[nPosicions++] = Posicio(saltoFila, saltoCol);
					}
				}
			}
		}
	}
	else if (fitxa.getTipus() == TIPUS_DAMA) {
		// Para cada diagonal
		for (int i = 0; i < 4; i++) {
			int newFila = fila + movs[i][0];
			int newCol = col + movs[i][1];
			bool encontrado = false;

			while (newFila >= 0 && newFila < 8 && newCol >= 0 && newCol < 8 && !encontrado) {
				Fitxa dest = m_tauler[newFila][newCol];

				if (dest.getTipus() == TIPUS_EMPTY) {
					posicionsPossibles[nPosicions++] = Posicio(newFila, newCol);
				}
				else if (dest.getColor() != fitxa.getColor() && dest.getTipus() != TIPUS_EMPTY) {
					int saltoFila = newFila + movs[i][0];
					int saltoCol = newCol + movs[i][1];
					if (saltoFila >= 0 && saltoFila < 8 && saltoCol >= 0 && saltoCol < 8
						&& m_tauler[saltoFila][saltoCol].getTipus() == TIPUS_EMPTY) {
						posicionsPossibles[nPosicions++] = Posicio(saltoFila, saltoCol);
					}
					encontrado = true; // dama solo puede saltar una vez en cada dirección por turno
				}
				else {
					encontrado = true;
				}
				newFila += movs[i][0];
				newCol += movs[i][1];
			}
		}
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

//bufar per no fer el millor mov en general, bufem la fitxa q podia fer el mov
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

	sortida += "  ";

	for(int i = 0; i < N_COLUMNES ; i++)
	{
		pos = 'a' + i;
		sortida += " " + pos;
	}

	return sortida;
}

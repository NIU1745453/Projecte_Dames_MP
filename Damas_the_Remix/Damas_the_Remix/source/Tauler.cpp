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
					getPosicionsImmediates(posicioActual, nPos, posicionsPossibles);

					if (nPos > 0)
					{
						for (int i = 0; i < nPos; i++)
						{
							Moviment nouMoviment = movActual;
							int nMov = nouMoviment.getnMoviment();

							// Calcular capturas para este movimiento
							int captures = 0;
							int damesCapturades = 0;
							calcularCapturas(posicioActual, posicionsPossibles[i], captures, damesCapturades);

							nouMoviment.setMoviment(posicionsPossibles[i].toString());
							nouMoviment.setnMoviment(nMov + 1);
							nouMoviment.setCaptures(nouMoviment.getCaptures() + captures);
							nouMoviment.setDamesCapturades(nouMoviment.getDamesCapturades() + damesCapturades);

							if (nPendents < MAX_MOVIMENTS)
								movPendents[nPendents++] = nouMoviment;
						}
					}
					else
					{
						if (movActual.getnMoviment() > 1 && nValids < MAX_MOVIMENTS)
							movValids[nValids++] = movActual;
					}

				} while (nPendents > 0);

				// Ordenar movimientos por número de capturas (mayor primero)
				for (int i = 0; i < nValids - 1; i++) {
					for (int j = i + 1; j < nValids; j++) {
						if (movValids[i].getCaptures() < movValids[j].getCaptures()) {
							Moviment temp = movValids[i];
							movValids[i] = movValids[j];
							movValids[j] = temp;
						}
					}
				}

				m_tauler[fila][col].setMovimentsValids(movValids, nValids);
				m_tauler[fila][col].setnValidas(nValids);//he añadido esto
			}
		}
	}
}

void Tauler::calcularCapturas(const Posicio& origen, const Posicio& desti, int& captures, int& damesCapturades)
{
	int filaOrigen = origen.getFila();
	int colOrigen = origen.getColumna();
	int filaDesti = desti.getFila();
	int colDesti = desti.getColumna();

	// Solo hay captura si el movimiento es de más de una casilla
	if (abs(filaDesti - filaOrigen) > 1 || abs(colDesti - colOrigen) > 1)
	{
		int dirFila = (filaDesti > filaOrigen) ? 1 : -1;
		int dirCol = (colDesti > colOrigen) ? 1 : -1;

		int filaActual = filaOrigen + dirFila;
		int colActual = colOrigen + dirCol;

		while (filaActual != filaDesti || colActual != colDesti)
		{
			// Verificar si hay una ficha enemiga en esta posición
			if (m_tauler[filaActual][colActual].getTipus() != TIPUS_EMPTY &&
				m_tauler[filaActual][colActual].getColor() != m_tauler[filaOrigen][colOrigen].getColor())
			{
				captures++;
				if (m_tauler[filaActual][colActual].getTipus() == TIPUS_DAMA)
				{
					damesCapturades++;
				}
			}

			filaActual += dirFila;
			colActual += dirCol;
		}
	}
}

void Tauler::getPosicionsImmediates(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	int fila = origen.getFila();
	int col = origen.getColumna();
	nPosicions = 0;
	Fitxa fitxa = m_tauler[fila][col];

	// desplazamientos diagonales, miramos que las diagonales de nuestra fixa cumplan las condiciones (bacias o enemigas)
	int movs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

	if (fitxa.getTipus() == TIPUS_NORMAL)
	{
		int dir;
		//decidimos la direccion de la fitxa(si se mueve hacia arriba o hacia abajo)
		if (fitxa.getColor() == COLOR_BLANC)
			dir = -1;
		else
			dir = 1;
		//al ser una normal solo puede ir hacia delante, asi que no miramos las posiciones posteriores
		for (int i = 0; i < 2; i++)
		{
			int newFila = fila + dir;
			int newCol = col + movs[i][1];

			//selecionamos la nueva posicion de referencia y comprobamos que esté en los parametros del tablero

			if (newFila >= 0 && newFila < 8 && newCol >= 0 && newCol < 8)
			{
				Fitxa dest = m_tauler[newFila][newCol];
				// Si está vacío, es posible
				if (dest.getTipus() == TIPUS_EMPTY)
				{
					posicionsPossibles[nPosicions++] = Posicio(newFila, newCol);
				}
				// Si hay ficha contraria y detrás está vacío → posible salto, teniendo en cuenta que ha de seguir la direcion diagonal indicada
				else if (dest.getColor() != fitxa.getColor() && dest.getTipus() != TIPUS_EMPTY)
				{
					int saltoFila = newFila + dir;
					int saltoCol = newCol + movs[i][1];
					if (saltoFila >= 0 && saltoFila < 8 && saltoCol >= 0 && saltoCol < 8 && m_tauler[saltoFila][saltoCol].getTipus() == TIPUS_EMPTY)
					{
						posicionsPossibles[nPosicions++] = Posicio(saltoFila, saltoCol);
					}
				}
			}
		}
	}
	else if (fitxa.getTipus() == TIPUS_DAMA)
	{
		//en el caso de las Damas, pueden ir en cualquier direción (delante y atrás), asi que miramos las 4 posiciones cardinales
		for (int i = 0; i < 4; i++)
		{
			int newFila = fila + movs[i][0];
			int newCol = col + movs[i][1];
			bool encontrado = false;

			//mientras no haya mirado todas las posiciones o todos los saltos no saldra del bucle

			while (newFila >= 0 && newFila < 8 && newCol >= 0 && newCol < 8 && !encontrado)
			{
				Fitxa dest = m_tauler[newFila][newCol];

				//si esta bacia agrega el espacio immediatamente, si es del color contrario mira que la siguiente este bacia y sale del bucle
				// finalmente, si es del mismo color sale del while
				if (dest.getTipus() == TIPUS_EMPTY)
					posicionsPossibles[nPosicions++] = Posicio(newFila, newCol);
				else if (dest.getColor() != fitxa.getColor() && dest.getTipus() != TIPUS_EMPTY)
				{
					int saltoFila = newFila + movs[i][0];
					int saltoCol = newCol + movs[i][1];
					if (saltoFila >= 0 && saltoFila < 8 && saltoCol >= 0 && saltoCol < 8 && m_tauler[saltoFila][saltoCol].getTipus() == TIPUS_EMPTY)
						posicionsPossibles[nPosicions++] = Posicio(saltoFila, saltoCol);

					encontrado = true;
					// dama solo puede saltar una vez en cada dirección por turno
				}
				else
					encontrado = true;

				newFila += movs[i][0];
				newCol += movs[i][1];
			}
		}
	}
}

//falta añadir la cantidad de fitxas matadas (y la de damas) de manera que no afecte en la funcion actualitza mov vàlids
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) {
	int fila = origen.getFila();
	int col = origen.getColumna();
	nPosicions = 0;
	Fitxa fitxa = m_tauler[fila][col];

	int movs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

	if (fitxa.getTipus() == TIPUS_NORMAL) {
		int dir = (fitxa.getColor() == COLOR_BLANC) ? -1 : 1;
		for (int i = 0; i < 2; i++) {
			int newFila = fila + dir;
			int newCol = col + movs[i][1];

			if (newFila >= 0 && newFila < N_FILES && newCol >= 0 && newCol < N_COLUMNES) {
				Fitxa dest = m_tauler[newFila][newCol];
				if (dest.getTipus() == TIPUS_EMPTY) {
					posicionsPossibles[nPosicions++] = Posicio(newFila, newCol);
				}
				else if (dest.getColor() != fitxa.getColor()) {
					int saltoFila = newFila + dir;
					int saltoCol = newCol + movs[i][1];
					if (saltoFila >= 0 && saltoFila < N_FILES && saltoCol >= 0 && saltoCol < N_COLUMNES &&
						m_tauler[saltoFila][saltoCol].getTipus() == TIPUS_EMPTY) {
						posicionsPossibles[nPosicions++] = Posicio(saltoFila, saltoCol);

						// Verificar si después del salto hay otra ficha para capturar
						int nuevaFila = saltoFila;
						int nuevaCol = saltoCol;
						bool puedeContinuar = true;
						while (puedeContinuar) {
							puedeContinuar = false;
							for (int j = 0; j < 2; j++) {
								int nextFila = nuevaFila + dir;
								int nextCol = nuevaCol + movs[j][1];
								if (nextFila >= 0 && nextFila < N_FILES && nextCol >= 0 && nextCol < N_COLUMNES) {
									Fitxa nextDest = m_tauler[nextFila][nextCol];
									if (nextDest.getColor() != fitxa.getColor() && nextDest.getTipus() != TIPUS_EMPTY) {
										int nextSaltoFila = nextFila + dir;
										int nextSaltoCol = nextCol + movs[j][1];
										if (nextSaltoFila >= 0 && nextSaltoFila < N_FILES && nextSaltoCol >= 0 && nextSaltoCol < N_COLUMNES &&
											m_tauler[nextSaltoFila][nextSaltoCol].getTipus() == TIPUS_EMPTY) {
											// Agregar la posición final después de múltiples saltos
											posicionsPossibles[nPosicions++] = Posicio(nextSaltoFila, nextSaltoCol);
											nuevaFila = nextSaltoFila;
											nuevaCol = nextSaltoCol;
											puedeContinuar = true;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (fitxa.getTipus() == TIPUS_DAMA) {
		for (int i = 0; i < 4; i++) {
			int newFila = fila + movs[i][0];
			int newCol = col + movs[i][1];
			bool puedeSaltar = false;

			while (newFila >= 0 && newFila < N_FILES && newCol >= 0 && newCol < N_COLUMNES) {
				Fitxa dest = m_tauler[newFila][newCol];

				if (dest.getTipus() == TIPUS_EMPTY) {
					if (!posicioExistent(Posicio(newFila, newCol), nPosicions, posicionsPossibles)) {
						posicionsPossibles[nPosicions++] = Posicio(newFila, newCol);
					}
				}
				else if (dest.getColor() != fitxa.getColor()) {
					int saltoFila = newFila + movs[i][0];
					int saltoCol = newCol + movs[i][1];
					if (saltoFila >= 0 && saltoFila < N_FILES && saltoCol >= 0 && saltoCol < N_COLUMNES &&
						m_tauler[saltoFila][saltoCol].getTipus() == TIPUS_EMPTY) {
						Posicio saltoPos(saltoFila, saltoCol);
						if (!posicioExistent(saltoPos, nPosicions, posicionsPossibles)) {
							posicionsPossibles[nPosicions++] = saltoPos;
							// Continuar explorando en la misma dirección para múltiples capturas
							newFila = saltoFila;
							newCol = saltoCol;
							puedeSaltar = true;
							continue;
						}
					}
					break;
				}
				else {
					break;
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
	bool fet = true;
	bool best = true;
	bool trobat = false;
	bool otra = false;

	ColorFitxa color = m_tauler[origen.getFila()][origen.getColumna()].getColor();

	int i = 0;
	int j = 0;

	int x = 0;
	int y = 0;
	int k = 0;

	int mi = origen.getFila();
	int mj = origen.getColumna();

	if (m_tauler[desti.getFila()][desti.getColumna()].getTipus() != TIPUS_EMPTY)
	{
		fet = false;
	}
	else
	{
		int captures = m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getCaptures();
		int capturesD = m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getDamesCapturades();

		while (i < m_tauler[origen.getFila()][origen.getColumna()].getnValidas()  && best)//mira si la ficha tiene un camino mejor
		{
			if (m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment().getFila() == desti.getFila() &&
				m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getMoviment().getColumna() == desti.getColumna())
			{
				j = i;
				trobat = true;
			}
			if ((m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getCaptures() > captures) ||
				(m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getCaptures() == captures && m_tauler[origen.getFila()][origen.getColumna()].getMoviments(i).getDamesCapturades() > capturesD))
			{
				
				best = false;
			}
			i++;
		}

		if (trobat)//ahora hay que mirar el resro de fixas
		{
			if (color == COLOR_BLANC)
			{//para mirar si alguna ficha blanca tiene un camino mejor
				while (x < 8)
				{
					while (y < 8)
					{
						while (k < m_tauler[x][y].getnValidas())
						{
							if (m_tauler[x][y].getColor() == COLOR_BLANC)
							{
								if ((m_tauler[x][y].getMoviments(k).getCaptures() > captures) ||
									(m_tauler[x][y].getMoviments(k).getCaptures() == captures && m_tauler[x][y].getMoviments(k).getDamesCapturades() > capturesD))
								{
									captures = m_tauler[i][j].getMoviments(k).getCaptures();
									capturesD = m_tauler[i][j].getMoviments(k).getDamesCapturades();
									mi = x;
									mj = y;
									best = false;
									otra = true;
								}
							}


							k++;

						}
						k = 0;
						y++;
					}
					y = 0;
					x++;
				}
			}
			else
			{//para mirar si hay alguna ficha negra con un camino mejor
				while (x < 8)
				{
					while (y < 8)
					{
						while (k < m_tauler[x][y].getnValidas())
						{
							if (m_tauler[x][y].getColor() == COLOR_NEGRE)
							{
								if ((m_tauler[x][y].getMoviments(k).getCaptures() > captures) ||
									(m_tauler[x][y].getMoviments(k).getCaptures() == captures && m_tauler[x][y].getMoviments(k).getDamesCapturades() > capturesD))
								{
									captures = m_tauler[i][j].getMoviments(k).getCaptures();
									capturesD = m_tauler[i][j].getMoviments(k).getDamesCapturades();
									mi = x;
									mj = y;
									best = false;
									otra = true;
								}
							}


							k++;

						}
						k = 0;
						y++;
					}
					y = 0;
					x++;
				}
			}
		}



		if (trobat)
		{

			int m = origen.getFila();
			int n = origen.getColumna();
			bool noM = false;
			bool acabat = false;

			if ((m_tauler[m][n].getTipus() != TIPUS_DAMA) && ( (m-desti.getFila() ==-1 && n-desti.getColumna() == 1 )
				||(m - desti.getFila() == -1 && n - desti.getColumna() == -1) || (m - desti.getFila() == 1 && n - desti.getColumna() == 1)
				|| (m - desti.getFila() == 1 && n - desti.getColumna() == -1)))
			{//para que si solo se mueve una casilla que no entre al bucle ya que no tiene que matar a ninguna, este condicional no se aplica a las damas
				noM = true;
			}



			if (m_tauler[m][n].getTipus() != TIPUS_DAMA && noM == false)//sirve para matar a las fixas contrarias, si la fixa que se mueve es una dama se utiliza el siguiente if ya que su movimiento es diferente
			{



				while (acabat == false && j > 0)
				{
					acabat = true;
					if (m_tauler[m][n].getMoviments(j-1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
						m_tauler[m][n].getMoviments(j-1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
					{//arriba izquierda
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
						acabat = false;
					}
					
					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
					{//arriba derecha
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
						acabat = false;

					}

					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
					{//abajo izquierda
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
						acabat = false;

					}

					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
					{//abajo derecha
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
						acabat = false;

					}

					j--;
				}

				if (acabat)
				{
					j++;
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
				{//arriba izquierda
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
				{//arriba derecha
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
				{//abajo izquierda
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
				{//abajo derecha
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
				}
			}

			if (m_tauler[origen.getFila()][origen.getColumna()].getTipus() == TIPUS_DAMA)//para las piezas que sean damas va eliminando las fichas
			{

				while (acabat == false && j > 0)
				{
					acabat = true;
					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
					{//arriba izquierda
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
						acabat = false;
					}

					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
					{//arriba derecha
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
						acabat = false;

					}

					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
					{//abajo izquierda
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
						acabat = false;

					}

					if (m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
					{//abajo derecha
						m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
						acabat = false;

					}

					if ((m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -1 &&
						m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 1) ||
						(m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -1 &&
							m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -1) ||
						(m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 1 &&
							m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 1) ||
						(m_tauler[m][n].getMoviments(j - 1).getMoviment().getFila() - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 1 &&
							m_tauler[m][n].getMoviments(j - 1).getMoviment().getColumna() - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -1))
					{//para si se mueve de uno en uno que no salga del bucle y mate a las fichas
						acabat = false;
					}

					j--;
				}
				if (acabat)
				{
					j++;
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
				{//arriba izquierda
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == -2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
				{//arriba derecha
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() - 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == 2)
				{//abajo izquierda
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() + 1] = Fitxa();
				}

				if (m - m_tauler[m][n].getMoviments(j).getMoviment().getFila() == 2 &&
					n - m_tauler[m][n].getMoviments(j).getMoviment().getColumna() == -2)
				{//abajo derecha
					m_tauler[m_tauler[m][n].getMoviments(j).getMoviment().getFila() + 1][m_tauler[m][n].getMoviments(j).getMoviment().getColumna() - 1] = Fitxa();
				}


			}

			if (best)//el movimiento que hace la fitxa si el moviemirnto es el mejor posible
			{
				if ((desti.getFila() == 8 && color == COLOR_BLANC) || (desti.getFila() == 1 && color == COLOR_NEGRE))
				{
					m_tauler[desti.getFila()][desti.getColumna()] = Fitxa(TIPUS_DAMA, m_tauler[origen.getFila()][origen.getColumna()].getColor(), desti.getColumna(), desti.getFila());
					m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
				}
				else
				{
					m_tauler[desti.getFila()][desti.getColumna()] = Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor(), desti.getColumna(), desti.getFila());
					m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();
				}
			}
			else//si no es el mejor movimiento bufa la pieza que si tenga el mejor movimiento
			{
				if(otra == false)
				{ 
					m_tauler[mi][mj] = Fitxa(); 
				}
				else
				{
					m_tauler[desti.getFila()][desti.getColumna()] = Fitxa(m_tauler[origen.getFila()][origen.getColumna()].getTipus(), m_tauler[origen.getFila()][origen.getColumna()].getColor(), desti.getColumna(), desti.getFila());
					m_tauler[mi][mj] = Fitxa();
					m_tauler[origen.getFila()][origen.getColumna()] = Fitxa();

				}
			}
		}
		else
		{
			fet = false;
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

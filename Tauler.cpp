#include "Tauler.h"
#include <fstream>

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

//void Tauler::actualitzaMovimentsValids()
//{
//	int i;
//	for (i = 0; i < 64; i++)
//	{
//
//	}
//}

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
						if(m_tauler[i][j].getTipus() == TIPUS_EMPTY)
							posicionsPossibles[nPosicions++] = newPos; //si es una posicion bacia la añade sin mas
						else if (m_tauler[i][j].getColor() != colorOrig && perVisitar[pos] != m_tauler[i][j].getColor()) //si es de un color diferente y la anterior es del color de la fitxa en juego la mete en el array para visitar
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
Moviment Tauler::camiViable(Posicio& posActual)
{
	//ALGORISME PER MIRAR ELS MOVIMENTS QUE ES PODEN FER
	int i = 0, x = 1;
	int nPos, posVal;

	Moviment movPendents[MAX_MOVIMENTS], movimentPendent[MAX_MOVIMENTS];
	Moviment posValides[MAX_MOVIMENTS];
	Moviment movimentActual[MAX_MOVIMENTS];
	movPendents[0] = movimentActual;
	Posicio posicioActual = posActual;

	for (int y = 0; y < MAX_MOVIMENTS; y++)
	{
		movPendents[y].setMoviment('', y);
		movimentPendent[y].setMoviment('', y);
		posValides[y].setMoviment('', y);
		movimentActual[y].setMoviment('', y);

	}

	Moviment movValids[MAX_MOVIMENTS]; //se le tendra q cambiar el nombre para adaptarlo a la actualizacion de moviment.h

	do
	{
		movimentActual[0] = movPendents[i];
		getPosicionsPossibles(posicioActual, nPos, posValides);

		while (posValides != [] && x < MAX_MOVIMENTS)
		{
			movimentPendent = movimentActual;
			movimentActual[x] = posValides[0];
			for (int j = 0; j < nPos; j++)
			{
				movimentPendent[x] = posValides[j + 1];
				movPendents[j] = movimentPendent;
			}
			posicioActual = movimentActual[x];
			getPosicionsPossibles(posicioActual, nPos, posValides);
			x++
		}
		x = 0;
		if (movimentActual != [])
			movValids[i] = movimentActual;

		i++;

	} while (movimentsPendents != [] && i < MAX_MOVIMENTS);

	return movValids;


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

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{

}

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
	for (int i = 0; i > N_COLUMNES; i--)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			cout << fil << ":" << ' ';
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

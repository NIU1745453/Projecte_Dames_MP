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
	ColorFitxa colorOrig = m_tauler[fila][columna].getColor();
	Posicio perVisitar[MAX_MOVIMENTS];
	int Visitar = 0;

	while ((newPos.getColumna() <= 7 && newPos.getColumna() >= 0) && (newPos.getFila() <= 7 && newPos.getFila() >= 0))
	{
		for (int i = (newPos.getFila()); i < N_FILES; i += 2)
		{
			for (int j = (newPos.getColumna()); i < N_COLUMNES; i += 2)
			{
				if (i < 8 && i> 0 && j < 8 && j > 0)
				{
					if (m_tauler[i][j].getTipus() == TIPUS_EMPTY)
					{
						newPos.setPosicio(i, j);
						if (!posicioExistent(newPos, nPosicions, posicionsPossibles))
						{
							if(m_tauler[i][j].getTipus() == TIPUS_EMPTY)
								posicionsPossibles[nPosicions++] = newPos;
							else if (m_tauler[i][j].getColor() != colorOrig)
							{
								perVisitar[Visitar] = newPos;
							}
						}

					}
				}
			}
		}
		pos++;
		newPos = perVisitar[pos];
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

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{

}

void Tauler::ToString(const string& nomFitxer, const Posicio& posicio)
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

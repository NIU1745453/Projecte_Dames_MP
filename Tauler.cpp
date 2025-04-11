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

void Tauler::actualitzaMovimentsValids()
{

}
x
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{

}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{

}

string Tauler::toString() const
{
	string pos = "a1";
	pos[0] = 'a' + m_columna;
	pos[1] = '1' + (N_FILES - 1) - m_fila;
	return pos;
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

//#ifndef MOVIMENT_H
//#define MOVIMENT_H

#include "posicio.hpp"

const int MAX_PECES = 24;

class Moviment
{
public:
	Moviment() {
		for (int i = 0; i < 50; i++)
		{
			m_posicio[i] = Posicio();
		}
		m_nPosicio = 0;
	}

	string getMoviment(int i) const { return m_posicio[i].getPosicio(); }
	int getnMoviment() { return m_nPosicio; }

	void setMoviment(string mov, int i) { m_posicio[i].setPosicio(mov); }
	void setnMoviment(int i) { m_nPosicio = i; }


private:
	Posicio m_posicio[50];//tiene variable fila, columna y string
	int m_nPosicio;
};

//#endif 
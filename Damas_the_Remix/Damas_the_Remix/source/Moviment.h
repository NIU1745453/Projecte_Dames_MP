#ifndef MOVIMENT_H_
	#define MOVIMENT_H_

#include "posicio.hpp"


const int MAX_PECES = 24;

class Moviment
{
public:
	Moviment() {
		for (int i = 0; i < 10; i++)
		{
			Posicio pos;
			m_posicio = pos;
		}
		m_nPosicio = 0;
	}

	Posicio getMoviment() const { return m_posicio; }
	int getnMoviment() { return m_nPosicio; }

	void setMoviment(string mov) { m_posicio.setPosicio(mov); }
	void setnMoviment(int i) { m_nPosicio = i; }


private:
	Posicio m_posicio;//tiene variable fila, columna y string

	int m_nPosicio;
};

#endif 
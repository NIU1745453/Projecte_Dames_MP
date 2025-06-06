#ifndef MOVIMENT_H_
	#define MOVIMENT_H_

#include "posicio.hpp"


const int MAX_PECES = 24;

class Moviment
{
public:
	Moviment() {
		//for (int i = 0; i < 10; i++)
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

	bool contienePosicion(const Posicio& pos) const
	{
		bool trobat = false;
		for (int i = 0; i < m_nPosicio && trobat == false; i++)
		{
			if (m_posicio == pos) //m_posicio se tendra que cambiar con lo que se ponga de camino o se tendra que implementar como vector
				trobat = true;
		}

		return trobat;
	}

	void agregarPosicion(const Posicio& pos)
	{
		m_posicio = pos;
		//m_posicio[m_nPosicio++] = pos;
	}

	Posicio getUltimaPosicion() const
	{
		return m_posicio;
		//return m_posicio[m_nPosicio - 1];
	}

private:
	Posicio m_posicio;//tiene variable fila, columna y string
	//m_posicio se tendira q convertir en classe vector
	int m_nPosicio;
};

#endif 
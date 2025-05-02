#ifndef FITXA_H
	#define FITXA_H
#include "Moviment.h"
#include "posicio.hpp"
//#ifndef FITXA_H
//#define FITXA_H



using namespace std;

const int MAX_MOVIMENTS = 12;//pot ser canviat

typedef enum
{
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
	COLOR_NEGRE,
	COLOR_BLANC,
	CAP_COLOR // afegit per a inicialitzar les fitxes abans de donar color (place holder)
} ColorFitxa;

class Fitxa
{
public:
	Fitxa() { m_tipus = TIPUS_EMPTY, m_color = CAP_COLOR, m_posicio.setPosicio(0, 0), m_nValidas = 0; 
	for (int i = 0; i < MAX_MOVIMENTS; i++)
	{
		m_movimentsValids[i] = Moviment();
	}
	}
	Fitxa(TipusFitxa tipus, ColorFitxa color, int vert, int horit) { m_tipus = tipus, m_color = color, m_posicio.setPosicio(horit, vert); }


	int getPosVert() { return m_posicio.getColumna(); }
	int getPosHorit() { return m_posicio.getFila(); }
	Posicio getPosicio() { return m_posicio; }
	int getnValidas() { return m_nValidas; }
	TipusFitxa getTipus() const { return m_tipus; }
	ColorFitxa getColor() const { return m_color; }
	Moviment getMoviments(int i) { return m_movimentsValids[i]; }

	void setPosFitxa(int vert, int horit) { m_posicio.setPosicio(vert, horit); }
	void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
	void setColor(ColorFitxa color) { m_color = color; }
	void setnValidas(int validas) { m_nValidas = validas; }

	//Moviment movimentsValids(Fitxa tauler[N_FILES][N_COLUMNES]) {};//encara no se que colocar aqui com a paramentres

private:
	TipusFitxa m_tipus;
	ColorFitxa m_color;
	Posicio m_posicio;
	Moviment m_movimentsValids[MAX_MOVIMENTS];
	int m_nValidas;
};

#endif 

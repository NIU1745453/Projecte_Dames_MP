#include <iostream>
#include <string.h>
#include "Moviment.h"
#include "Tauler.h"
#include "Posicio.h"

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
	Fitxa() { m_tipus = TIPUS_EMPTY, m_color = CAP_COLOR, m_posicio.setPosicio(0, 0); 
	for (int i = 0; i < MAX_MOVIMENTS; i++)
	{
		m_movimentsValids[i].
	}
	}
	Fitxa(TipusFitxa tipus, ColorFitxa color, int vert, int horit) { m_tipus = tipus, m_color = color, m_posicio.setPosicio(horit, vert); }


	int getPosVert() { return m_posicio.getColumna(); }
	int getPosHorit() { return m_posicio.getFila(); }
	TipusFitxa getTipus() { return m_tipus; }
	ColorFitxa getColor() { return m_color; }

	void setPosFitxa(int vert, int horit) { m_posicio.setPosicio(vert, horit); }
	void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
	void setColor(ColorFitxa color) { m_color = color; }

	Moviment movimentsValids(Fitxa tauler[N_FILES][N_COLUMNES]) {};//encara no se que colocar aqui com a paramentres

private:
	TipusFitxa m_tipus;
	ColorFitxa m_color;
	Posicio m_posicio;
	Moviment m_movimentsValids[MAX_MOVIMENTS];
};

#include <iostream>
#include <string.h>
#include "Moviment.h"

using namespace std;

const int MAX_MOVIMENTS = 10;//pot ser canviat

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
	Fitxa() : m_tipus(TIPUS_EMPTY), m_color(COLOR_NEGRE), m_posVert(0), m_posHorit(0) {}
	Fitxa(TipusFitxa tipus, ColorFitxa color, int vert, int horit) : m_tipus(tipus), m_color(color), m_posVert(vert), m_posHorit(horit){}


	int getPosVert() { return m_posVert; }
	int getPosHorit() { return m_posHorit; }
	TipusFitxa getTipus() { return m_tipus; }
	ColorFitxa getColor() { return m_color; }
	Posicio getPosicio() { return m_posicioActual; }

	void setPosFitxa(int vert, int horit) { m_posVert = vert; m_posHorit = horit; }
	void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
	void setColor(ColorFitxa color) { m_color = color; }

	Moviment movimentsValids();//encara no se que colocar aqui com a paramentres

private:
	TipusFitxa m_tipus;
	ColorFitxa m_color;
	int m_posVert;
	int m_posHorit;
	Moviment m_movimentsValids[MAX_MOVIMENTS];
};

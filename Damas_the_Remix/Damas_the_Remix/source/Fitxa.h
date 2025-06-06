#ifndef FITXA_H
	#define FITXA_H
#include "Moviment.h"

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
	Fitxa() 
	{
		m_tipus = TIPUS_EMPTY;
		m_color = CAP_COLOR;
		m_matades = 0;
		m_damesMatades = 0;
		Posicio pos;
		m_posicio = pos;
		m_nValidas = 0;
		for (int i = 0; i < MAX_MOVIMENTS; i++)
		{
			Moviment mov;
			m_movimentsValids[i] = mov;
		}
	}
	Fitxa(TipusFitxa tipus, ColorFitxa color, int vert, int horit) 
	{
		m_tipus = tipus;
		m_color = color;
		m_posicio.setPosicio(horit, vert);
		m_matades = 0;
		m_damesMatades = 0;
	}

	int getPosVert() { return m_posicio.getColumna(); }
	int getPosHorit() { return m_posicio.getFila(); }
	Posicio getPosicio() { return m_posicio; }
	int getnValidas() { return m_nValidas; }
	TipusFitxa getTipus() const { return m_tipus; }
	ColorFitxa getColor() const { return m_color; }
	Moviment getMoviments(int i) { return m_movimentsValids[i]; }

	//fallan los setters

	void setFitxa(TipusFitxa tipus, ColorFitxa color, int vert, int horit)
	{
		m_tipus = tipus;
		m_color = color;
		m_posicio.setPosicio(horit, vert);
	}
	void setPosFitxa(int vert, int horit) { m_posicio.setPosicio(vert, horit); }
	void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
	void setColor(ColorFitxa color) { m_color = color; }
	void setnValidas(int validas) { m_nValidas = validas; }
	void setMovimentsValids(Moviment movValids[], int nMov)
	{
		for (int i = 0; i < nMov; i++)
		{
			m_movimentsValids[i] = movValids[i];
		}
	}
	//Moviment movimentsValids(Fitxa tauler[N_FILES][N_COLUMNES]) {};//encara no se que colocar aqui com a paramentres

private:
	TipusFitxa m_tipus;
	ColorFitxa m_color;
	Posicio m_posicio;
	Moviment m_movimentsValids[MAX_MOVIMENTS]; //esto tmb tendra q ser un vector
	int m_nValidas;
	int m_damesMatades; //en cas d'empat saber qui ha matat mes dames
	int m_matades; // per saber quina bufar
};

#endif 

#include "Fitxa.h"

//aquesta funcio ha de codificar tots els moviments que podra fer la fitxa des de la seva posicio acual
//respecte el tauler sencer i haura de guardar en un array un conjunt d'arrays que guardaran els
//possibles moviments que pot fer la fitxa
//P(2a) = [(3c, 4a), (3c,4e)], este formato vamos
Moviment Fitxa::movimentsValids(Fitxa tauler[N_FILES][N_COLUMNES])
{
	int i = 0, x=0;
	bool continuar = true;
	Fitxa jugadas1[24];
	Fitxa jugadas2[24];

	if (m_posicio.getColumna() != 8)//podria ser 7 no estpy segura
	{
		if (tauler[m_posicio.getColumna() + 1][m_posicio.getFila() + 1].getColor == CAP_COLOR)
		{
			jugadas1[i].setPosFitxa(m_posicio.getColumna() + 1, m_posicio.getFila() + 1);
			continuar = false;
		}
		while(continuar)
		{
			
		}
		continuar = true;
	}
	if (m_posicio.getColumna() != 1)//podria ser 0 no estoy segura
	{

	}
}
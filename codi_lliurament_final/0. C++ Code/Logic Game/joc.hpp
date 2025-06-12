#ifndef JOC_H
#define JOC_H

#include <stdio.h>
#include "info_joc.hpp"
#include "tauler.hpp"
//afegits per mi
#include <vector>
using namespace std;


class Joc 
{

public:
    Joc() : m_enMenuInici(true), m_mouseStatusPrev(false), m_mousePosXPrev(0), m_mousePosYPrev(0) {};   
    void inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments);
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    void finalitza();
    bool mostrarMenuInici(int mousePosX, int mousePosY, bool mouseStatus);
    bool estaEnMenuInici() const { return m_enMenuInici; }
    
private:
    Tauler m_tauler;
    //vector<string> m_movPendentsNormal;   
            //recordar que en el cas de salts multiples el format sera a8,b7,b7,c8 (exemple) 
            // y que per tant aixo s'haura de tenir en compte al actualitzar el joc
    ModeJoc m_mode;
    string m_fitxerMov;
    int m_fitxerMovNLine;
    bool m_fitxaSeleccionada;
    Posicio m_posSeleccionada;
    vector<Posicio> m_movPendents; // Para guardar movimientos en modo normal
    vector<Posicio> m_posicionsPossibles;
    bool m_mouseStatusPrev; // Estado anterior del botón del ratón
    int m_mousePosXPrev;    // Posición X anterior del ratón
    int m_mousePosYPrev;    // Posición Y anterior del ratón
    bool m_enMenuInici;
};

#endif 

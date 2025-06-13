//
//  CurrentGame.cpp
//  LearnChess
//
//  Created by Enric Vergara on 21/2/22.
//

#include "joc.hpp"
#include <iostream>
#include <fstream>
#include "GraphicManager.h"

// Función para mostrar el menú de inicio y detectar clics
bool Joc::mostrarMenuInici(int mousePosX, int mousePosY, bool mouseStatus) 
{
    GraphicManager::getInstance()->drawSprite(GRAFIC_PANTALLA_INICI, 0, 0);

    const int newGameX1 = 250, newGameX2 = 500;
    const int newGameY1 = 425, newGameY2 = 525;
    const int replayX1 = 250, replayX2 = 500;
    const int replayY1 = 550, replayY2 = 625;

    bool mouseJustPressed = (mouseStatus && !m_mouseStatusPrev);

    if (mouseJustPressed) 
    {
        if (mousePosX >= newGameX1 && mousePosX <= newGameX2 && mousePosY >= newGameY1 && mousePosY <= newGameY2) 
        {
            m_mode = MODE_JOC_NORMAL;
            m_enMenuInici = false;
            inicialitza(m_mode, "data/Games/tauler_inicial.txt", "data/Games/moviments.txt");
            return true;
        }
        else if (mousePosX >= replayX1 && mousePosX <= replayX2 && mousePosY >= replayY1 && mousePosY <= replayY2) 
        {
            m_mode = MODE_JOC_REPLAY;
            m_enMenuInici = false;
            inicialitza(m_mode, "data/Games/tauler_inicial.txt", "data/Games/moviments.txt");
            return true;
        }
    }

    // Actualizar el estado anterior del ratón
    m_mouseStatusPrev = mouseStatus;
    m_mousePosXPrev = mousePosX;
    m_mousePosYPrev = mousePosY;

    return false;
}

void Joc::inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments)
{
	m_tauler.inicialitza(nomFitxerTauler);
	m_fitxerMov = nomFitxerMoviments; //guardem el nom d'aquest fitxer per a usarlo com a mesura més endavant
	m_fitxerMovNLine = 0;

	if (mode == MODE_JOC_REPLAY)
	{
		ifstream fitxer(nomFitxerMoviments);
		Posicio original;
		Posicio final;

		//recordar que en el cas de salts multiples el format sera a8,b7,b7,c8 (exemple) y que per tant aixo s'haura de tenir en compte al actualitzar el joc
		while (!fitxer.eof())
		{
			fitxer >> original >> final;
			m_movPendents.push_back(original);
			m_movPendents.push_back(final);
			m_fitxerMovNLine++; //d'aquesta manera a la hora de fer el mode replay sabrem quants clics del jugador ha de durar
		}

		fitxer.close();
	}
 
}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus) 
{
    // Dibujar el fondo y el tablero vacío
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);

    // Dibujar todas las fichas en sus posiciones actuales
    m_tauler.visualitza();

    // Detectar si el botón del ratón acaba de ser presionado (evita múltiples activaciones)
    bool mouseJustPressed = (mouseStatus && !m_mouseStatusPrev);

    if (m_mode == MODE_JOC_REPLAY) 
    {
        // --- MODO REPLAY ---
        string modoJuego = "Mode: REPLAY";
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, POS_Y_TAULER - 30, 0.8, modoJuego);

        if (mouseJustPressed && !m_movPendents.empty()) 
        {
            // Procesar un movimiento completo (origen y destino) por clic
            if (m_movPendents.size() >= 2) 
            {
                Posicio origen = m_movPendents[0];
                Posicio desti = m_movPendents[1];

                // Mover la ficha y actualizar el tablero
                m_tauler.mouFitxa(origen, desti);
                m_tauler.canviaTorn();
                // Eliminar los movimientos procesados
                m_movPendents.erase(m_movPendents.begin(), m_movPendents.begin() + 2);

                // Comprobar si el juego ha terminado
                if (m_tauler.jocAcabat()) 
                {
                    string guanyador = (m_tauler.getGuanyador() == COLOR_BLANC) ? "BLANQUES" : "NEGRES";
                    string mensageFinal = "FI DEL JOC - GUANYEN LES " + guanyador;
                    GraphicManager::getInstance()->drawFont(FONT_RED_30, POS_X_TAULER, POS_Y_TAULER + 700, 1.0, mensageFinal);
                    return true; // Indica que el juego ha terminado
                }
            }
        }
        else if (m_movPendents.empty()) 
            GraphicManager::getInstance()->drawFont(FONT_RED_30, POS_X_TAULER, POS_Y_TAULER + 700, 1.0, "No queden més moviments per reproduir");
    }
    else if (m_mode == MODE_JOC_NORMAL) 
    {
        // --- MODO NORMAL ---
        string modoJuego = "Mode: NORMAL - Torn: ";
        modoJuego += (m_tauler.getTorn() == COLOR_BLANC) ? "BLANQUES" : "NEGRES";
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, POS_Y_TAULER - 30, 0.8, modoJuego);

        // Verificar si el clic está dentro del tablero
        if (mouseJustPressed &&
            mousePosX >= (POS_X_TAULER + CASELLA_INICIAL_X) &&
            mousePosY >= (POS_Y_TAULER + CASELLA_INICIAL_Y) &&
            mousePosX <= (POS_X_TAULER + CASELLA_INICIAL_X + AMPLADA_CASELLA * NUM_COLS_TAULER) &&
            mousePosY <= (POS_Y_TAULER + CASELLA_INICIAL_Y + ALCADA_CASELLA * NUM_FILES_TAULER)) 
        {

            // Calcular la posición en el tablero (fila y columna)
            int columna = (mousePosX - (POS_X_TAULER + CASELLA_INICIAL_X)) / AMPLADA_CASELLA;
            int fila = (mousePosY - (POS_Y_TAULER + CASELLA_INICIAL_Y)) / ALCADA_CASELLA;
            Posicio posClicada(fila, columna);

            // --- LÓGICA DE SELECCIÓN Y MOVIMIENTO ---
            if (!m_fitxaSeleccionada) 
            {
                // Si no hay ficha seleccionada, verificar si se clicó una ficha del turno actual
                if (m_tauler.esFitxaDelColor(posClicada, m_tauler.getTorn())) 
                {
                    m_fitxaSeleccionada = true;
                    m_posSeleccionada = posClicada;

                    // Obtener posiciones posibles para la ficha seleccionada
                    int nPosicions = 0;
                    Posicio posicionsPossibles[MAX_MOVIMENTS];
                    m_tauler.getPosicionsPossibles(m_posSeleccionada, nPosicions, posicionsPossibles);

                    m_posicionsPossibles.clear();

                    for (int i = 0; i < nPosicions; i++) 
                        m_posicionsPossibles.push_back(posicionsPossibles[i]);
                }
            }
            else 
            {
                // Si hay ficha seleccionada, verificar si el destino es válido
                bool movimentValid = false;
                for (int i = 0; i < m_posicionsPossibles.size(); ++i)
                {
                    if (m_posicionsPossibles[i] == posClicada)
                    {
                        movimentValid = true;
                        break;
                    }
                }

                if (movimentValid) 
                {
                    // Mover la ficha y guardar el movimiento en el archivo
                    if (m_tauler.mouFitxa(m_posSeleccionada, posClicada)) 
                    {
                        m_tauler.canviaTorn();
                        ofstream fitxer(m_fitxerMov);
                        if (fitxer.is_open()) 
                        {
                            fitxer << m_posSeleccionada.toString() << " " << posClicada.toString() << endl;
                            fitxer.close();
                        }

                        // Comprobar si el juego ha terminado
                        if (m_tauler.jocAcabat()) 
                        {
                            string guanyador = (m_tauler.getGuanyador() == COLOR_BLANC) ? "BLANQUES" : "NEGRES";
                            string mensageFinal = "FI DEL JOC - GUANYEN LES " + guanyador;
                            GraphicManager::getInstance()->drawFont(FONT_RED_30, POS_X_TAULER, POS_Y_TAULER + 700, 1.0, mensageFinal);
                            return true;
                        }

                        GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
                        m_tauler.visualitza();
                    }
                }

                // Restablecer la selección
                m_fitxaSeleccionada = false;
                m_posicionsPossibles.clear();
                m_tauler.netejaMovimentsValids();
            }
        }

        // Mostrar posiciones válidas si hay una ficha seleccionada
        if (m_fitxaSeleccionada) 
        {
            for (const auto& pos : m_posicionsPossibles) 
            {
                int posX = POS_X_TAULER + CASELLA_INICIAL_X + (pos.getColumna() * AMPLADA_CASELLA);
                int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (pos.getFila() * ALCADA_CASELLA);
                GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posX, posY);
            }
        }
    }

    // Mostrar posición del ratón (debug)
    string mouse = "Mouse: X=" + to_string(mousePosX) + ", Y=" + to_string(mousePosY);
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, POS_Y_TAULER + 750, 0.6, mouse);

    // Actualizar el estado anterior del ratón
    m_mouseStatusPrev = mouseStatus;
    m_mousePosXPrev = mousePosX;
    m_mousePosYPrev = mousePosY;

    return false;
}

void Joc::finalitza()
{
    // Solo guardamos movimientos si estamos en modo normal y hay movimientos pendientes
    if (m_mode == MODE_JOC_NORMAL && !m_movPendents.empty())
    {
        ofstream fitxer(m_fitxerMov);

        if (fitxer.is_open())
        {
            // Guardamos los movimientos en pares (origen, destino)
            for (int i = 0; i < m_movPendents.size(); i += 2)
            {
                if (i + 1 < m_movPendents.size()) // Aseguramos que tenemos par completo
                    fitxer << m_movPendents[i].toString() << " " << m_movPendents[i + 1].toString() << endl;
            }
            fitxer.close();
        }
    }

    // Limpiar cualquier estado pendiente
    m_movPendents.clear();
    m_posicionsPossibles.clear();
    m_fitxaSeleccionada = false;
    m_enMenuInici = true; // Volvemos al menú inicial
}



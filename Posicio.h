#include <iostream>
#include <string.h>
#include <sstream>
#include "Fitxa.h"
using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

class Posicio
{
public:
    Posicio(const string& posicio) { m_posicio = posicio; fromString(posicio); }
    Posicio() {
        m_fila = 0; m_columna = 0;
        m_posicio = toString();
    }

	bool operator==(const Posicio& posicio) const;

    void setPosicio(int fila, int columna) { m_fila = fila; m_columna = columna; m_posicio = toString(); }
    void setPosicio(string posicio) { m_posicio = posicio; fromString(posicio); }

    int getFila() const { return m_fila; }
    int getColumna() const { return m_columna; }
    string getPosicio() const { return m_posicio; }

    string toString() const;
    void fromString(const string& pos);

private:
    int m_fila, m_columna;
    string m_posicio;
};

stringstream& operator>>(stringstream& fitxer, Posicio& posicio);
//ofstream& operator<<(ofstream& fitxer, const Posicio& posicio);
stringstream& operator<<(stringstream& fitxer, const Posicio& posicio);

void llegeixFitxa(const string& nomFitxer, char& tipusFitxa, Posicio& posicio);
void escriuFitxa(const string& nomFitxer, char tipusFitxa, const Posicio& posicio);
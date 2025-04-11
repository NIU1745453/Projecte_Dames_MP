#include <iostream>
#include <string.h>
#include "Fitxa.h"
using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

class Posicio
{
public:
    Posicio(const string& posicio) : m_posicio(posicio) {}
	bool operator==(const Posicio& posicio) const;
    Posicio() { m_fila = 0; m_columna = 0; }
    Posicio(int fila, int columna) { m_fila = fila; m_columna = columna; }
    int getFila() const { return m_fila; }
    int getColumna() const { return m_columna; }
    string toString() const;
    void fromString(const string& pos);
private:
    int m_fila, m_columna;
    string m_posicio;
};

ifstream& operator>>(ifstream& fitxer, Posicio& posicio);
ofstream& operator<<(ofstream& fitxer, const Posicio& posicio);

void llegeixFitxa(const string& nomFitxer, char& tipusFitxa, Posicio& posicio);
void escriuFitxa(const string& nomFitxer, char tipusFitxa, const Posicio& posicio);
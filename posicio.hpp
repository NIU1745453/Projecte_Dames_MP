#ifndef POSICIO_H
#define POSICIO_H


#include <iostream>
using namespace std;



class Posicio
{
public:
    Posicio(const string& posicio) : m_posicio(posicio) {}
    Posicio() { m_fila = 0; m_columna = 0; }

    bool operator==(const Posicio& posicio) const;

    void setPosicio(int fila, int columna) { m_fila = fila; m_columna = columna; }
    void setPosicio(string posicio) { m_posicio = posicio; }

    int getFila() const { return m_fila; }
    int getColumna() const { return m_columna; }
    string getPosicio() const { return m_posicio; }

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

#endif 
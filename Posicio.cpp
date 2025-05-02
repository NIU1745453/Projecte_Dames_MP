#include "posicio.hpp"

const int N_FILES = 8;
const int N_COLUMNES = 8;

bool Posicio::operator==(const Posicio& posicio) const
{
    bool iguals = false;
    if (posicio.getFila() == m_fila && posicio.getColumna() == m_columna)
    {
        iguals = true;
    }
    return iguals;
}

string Posicio::toString() const
{
    string pos = "a1";
    pos[0] = 'a' + m_columna;
    pos[1] = '1' + (N_FILES - 1) - m_fila;
    return pos;
}

void Posicio::fromString(const string& pos)
{
    m_fila = (N_FILES - 1) - (pos[1] - '1');
    m_columna = pos[0] - 'a';

}

istream& operator>>(istream& fitxer, Posicio& posicio)
{
    string pos;
    fitxer >> pos;
    posicio.fromString(pos);
    return fitxer;

}
stringstream& operator<<(stringstream& fitxer, const Posicio& posicio)
{
    string pos = posicio.toString();
    fitxer << pos;
    return fitxer;
}
//
//void llegeixFitxa(const string& nomFitxer, char& tipusFitxa, Posicio& posicio)
//{
//    ifstream fitxer(nomFitxer);
//    fitxer >> tipusFitxa;
//    fitxer >> posicio;
//    fitxer.close();
//}
//
//void escriuFitxa(const string& nomFitxer, char tipusFitxa, const Posicio& posicio)
//{
//    ofstream fitxer(nomFitxer);
//    fitxer << tipusFitxa << ' ';
//    fitxer << posicio;
//    fitxer.close();
//}
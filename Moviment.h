#include <iostream>
#include <string>


class Moviment
{
public:
	Moviment() : m_moviment(""), m_horitzontal(0456), m_vertical(0){}
	Moviment(string mov) : m_moviment(mov) {}
	void getMoviment() { return m_moviments; }
	string setMoviment(string mov) { m_moviment = mov; }

	bool compatible(int vertical, int horitzontal); //mirara si el moviment que mira es pot fer o no,
													// en cas de que es pugui fer el mov, l'haura de guardar per
													//tal de poderlo tenir en l'array moviments possibles


private:
	string m_moviment;
	int m_horitzontal;
	int m_vertical;
};

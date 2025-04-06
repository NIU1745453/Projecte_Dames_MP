#include <iostream>
#include <string.h>
#include "Fitxa.h"
using namespace std;

class Posicio
{
public:
	Posicio(const string& posicio);
	bool operator==(const Posicio& posicio) const;
private:
};

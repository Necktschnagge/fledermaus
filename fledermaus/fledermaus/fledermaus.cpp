// fledermaus.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>

#include "shapes.h"

using std::cout;
using std::cin;

int main()
{	
	cout << "Fusselsoft Fledermaus\nVersion 0.01\nexperimental\n\n";
	maus::Position pos{0,0};
	cout << pos.x();
	cin.get();
    return 0;
}


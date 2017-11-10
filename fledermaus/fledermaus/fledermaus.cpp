// fledermaus.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "shapes.h"
#include "shape_package.h"

using std::cout;
using std::cin;
using std::cerr;

int main()
{	
	cout << "Fusselsoft Fledermaus\nVersion 0.01\nexperimental\n\n";
	cout << "Open basic shapes file...\n";
	std::ifstream basic_shapes_file{};
	basic_shapes_file.open("../fledermaus/stdshapes.msf.sad", std::ifstream::in);
	// check whether well opened file:
	if (basic_shapes_file.fail()) {
		cerr << "Could not open file.";
		cin.get();
		return 1;
	}
	cout << "Parse basic shapes file...\n";
	sad::File basic_shapes_file_sad{};
	try {
		basic_shapes_file_sad.open(basic_shapes_file);
	}
	catch (...) {
		cerr << "could not read sad format for some reason.\nrest of file is:\n";
		{
			char c;
			while (basic_shapes_file >> c) cout << c;
		}
	}
	basic_shapes_file.close();


	maus::ShapePackage package{};
	package.get_root_directory().add_items_from_sad(basic_shapes_file_sad.get_node());
	
	cin.get();
    return 0;
}


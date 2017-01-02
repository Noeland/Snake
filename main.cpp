//============================================================================
// Name        : Snake.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Field.h"
#include "Snake.h"

using namespace std;
int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	Field f;
	Snake s(&f);
	f.init(&s);

	f.display();
}

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
#include <unistd.h>

using namespace std;

#include <unistd.h>
#include <term.h>

void ClearScreen()
{
	if (!cur_term)
	{
		int result;
		setupterm( NULL, STDOUT_FILENO, &result );
		if (result <= 0) return;
	}
	putp(tigetstr("clear") );
}


int main() {

	Field f;
	Snake s(&f);
	f.init(&s);

	while(!s.Dead() && !f.youWin())
	{
		ClearScreen();
		f.display(s);
		s.move();
		f.move();
		usleep(1000* 200);
	}

	ClearScreen();
	f.display(s);

	if(f.youWin())
		cout << "Congratulation!" << endl;
	else
		cout << "That's bad!" << endl;
	cout << endl;


}

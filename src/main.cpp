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
#include "Utils.h"
#include <unistd.h>

using namespace std;

static bool pathMap[FIELD_SIZE];
inline void printMap(int arr[], Index p)
{
	int size = WIDTH * HEIGHT;
	for(int i=0; i!=size; i++) {
		if(i%WIDTH == 0)
			std::cout << std::endl;
		if(i == p || pathMap[i] == true) {
			pathMap[i]=true;
			std::cout << "\033[1;31m";
			std::cout.width(4); std::cout << std::right << arr[i] << "\033[0m";
		}
		else if(arr[i] != 0 && arr[i]!=-1) {
			std::cout << "\033[1;34m";
			std::cout.width(4); std::cout << std::right << arr[i] << "\033[0m";
		}
		else {
			std::cout.width(4); std::cout << std::right << arr[i];
		}
	}
	std::cout << std::endl;
}


int main() {
//
//	cout << "Please enter frequency (How many steps in one second): ";
//	int freq;
//	cin >> freq;
//	if(freq <= 0)
//		unix_error("Invalid frequency!");

	Field f;
	Snake s(&f);
	f.init(&s);

	int map[FIELD_SIZE];
	s.testDFS(s.getHeadIdx(), f.getFoodIdx(), &s.path, map);


	Index curr = s.getHeadIdx();
	while(!s.path.empty()) {
		ClearScreen();
		curr += s.path.top();
		printMap(map, curr);
		s.path.pop();
		usleep(1000*50);
	}

//
//	while(!s.Dead() && !f.youWin())
//	{
//		ClearScreen();
//		f.display(s);
//		s.move();
//		f.move();
//		usleep(1000* 1000/freq);
//	}
//
//	ClearScreen();
//	f.display(s);
//
//	if(f.youWin())
//		cout << "Congratulation!" << endl;
//	else
//		cout << "That's bad!" << endl;
//	cout << endl;



}

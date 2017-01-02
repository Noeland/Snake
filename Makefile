all:Snake
Snake: *.cpp *.h
	g++ --std=c++11 -g3 -O0 -Wall *.cpp -o snake

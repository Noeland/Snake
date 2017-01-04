/*
 * Snake.h
 *
 *  Created on: Dec 31, 2016
 *      Author: Apple
 */

#ifndef SNAKE_H_
#define SNAKE_H_
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include "Utils.h"

struct Field;
struct Snake
{
	Snake(Field* f);
	Snake(const Snake& other);
	Snake& operator=(const Snake& other);
	// Snake(Direc dir, unsigned len);
	inline Index getHeadIdx() const;
	inline Index getTailIdx() const;
	inline Direc getCurrDir() const;
	inline unsigned getLength() const;
	bool Dead() { return isDead; }
	void move();
//private:
	void moveTo(bool isVirtual=false);			// move according to current direction.
	void grow(bool isVirtual=false);
	bool isDeadMove(Direc Dir) const;
	inline void popTail();
	inline void insertHead(Index newhead);
	unsigned myBFS(Index start, Index end, std::stack<Direc> *Path);
	unsigned DFS(Index start, Index end, std::stack<Direc> *Path);
	unsigned testDFS(Index start, Index end, std::stack<Direc> *Path, int lenMap[]);
	unsigned testBFS(Index start, Index end, std::stack<Direc> *Path, int lenMap[]);
	void chaseTail();



	std::list<Index> snake;
	bool isDead;
	Direc currDir;
	unsigned length;
	Field *field;
	std::stack<Direc> path;

};

inline Index Snake::getHeadIdx() const
{
	return snake.back();
}

inline Index Snake::getTailIdx() const
{
	return snake.front();
}

inline void Snake::popTail()
{
	snake.pop_front();
}

inline void Snake::insertHead(Index newhead)
{
	snake.push_back(newhead);
}

inline Direc Snake::getCurrDir() const
{
	return currDir;
}

inline unsigned Snake::getLength() const
{
	return length;
}


inline void printMap(int arr[])
{
	int size = WIDTH * HEIGHT;
	for(int i=0; i!=size; i++) {
		if(i%WIDTH == 0)
			std::cout << std::endl;
		if(arr[i] != 0 && arr[i]!=-1) {
			std::cout << "\033[1;34m";
			std::cout.width(4); std::cout << std::right << arr[i] << "\033[0m";
		}
		else {
			std::cout.width(4); std::cout << std::right << arr[i];
		}
	}
	std::cout << std::endl;
}
#endif /* SNAKE_H_ */

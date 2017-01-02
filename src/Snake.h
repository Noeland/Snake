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
private:
	void moveTo(bool isVirtual=false);			// move according to current direction.
	void grow(bool isVirtual=false);
	bool isDeadMove(Direc Dir) const;
	inline void popTail();
	inline void insertHead(Index newhead);
	unsigned BFS(Index start, Index end, std::stack<Direc>& Path);
	unsigned DFS(Index start, Index end, std::stack<Direc>& Path);



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

#endif /* SNAKE_H_ */

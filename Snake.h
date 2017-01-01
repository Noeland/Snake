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
#include "Utils.h"

struct Field;
struct Snake
{
	Snake(Field* f);
	Snake(const Snake& other);
	// Snake(Direc dir, unsigned len);
	inline Index getHeadIdx() const;
	inline Index getTailIdx() const;
private:
	void moveTo();			// move according to current direction.
	void grow();
	bool isDeadMove(Direc Dir) const;
	inline void popTail();
	inline void insertHead(Index newhead);

	std::list<Index> snake;
	bool isDead;
	Direc currDir;
	unsigned length;
	Field *field;

};

inline Index Snake::getHeadIdx() const
{
	return snake.front();
}

inline Index Snake::getTailIdx() const
{
	return snake.back();
}

inline void Snake::popTail()
{
	snake.pop_front();
}

inline void Snake::insertHead(Index newhead)
{
	snake.push_back(newhead);
}


#endif /* SNAKE_H_ */

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

const Index INIT_IDX = getIdx(1,1);

struct Field;
struct Snake
{
	Snake();
	Snake(const Snake& other);
	Snake(Direc dir, unsigned len);
	Index getHeadIdx();
	Index getTailIdx();
private:
	void moveTo();			// move according to current direction.
	void grow();
	bool isDeadMove(Direc Dir);

	std::list<Index> snake;
	bool isDead;
	Direc currDir;
	unsigned length;
	Field *field;

};





#endif /* SNAKE_H_ */

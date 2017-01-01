/*
 * Field.h
 *
 *  Created on: Dec 31, 2016
 *      Author: Apple
 */

#ifndef FIELD_H_
#define FIELD_H_

#include "Utils.h"

const Direc UP = -22;
const Direc DOWN = 22;
const Direc RIGHT = 1;
const Direc LEFT = -1;

const field_elem EMPTY_SLOT = ' ';
const field_elem FOOD = '@';
const field_elem WALL = 'X';

struct Snake;
struct Field
{
public:
	Field();
	void init(const Snake* snake);
	void move();
	unsigned fieldSize();
	field_elem getElem(Index idx);
	bool isFood(Index idx);
	void moveSnake(const Snake* snake);		// draw new snake location of field
	void growSnake(const Snake* snake);

private:
	void eraseFood();		// set the field[food index] to be empty
	void foodGen();			// generate new food index and set field[new index] to be the food
	void foodEaten();		// set hasBeenEaten to be true


/*	static const unsigned WIDTH = 22;
	static const unsigned HEIGHT = 12;*/
	Index field[WIDTH * HEIGHT];
	bool hasBeenEaten;
	Index foodIdx;

};



#endif /* FIELD_H_ */

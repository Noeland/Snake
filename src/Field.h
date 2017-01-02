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
const field_elem SNAKE_BODY = '*';
const unsigned FIELD_SIZE = WIDTH * HEIGHT;

struct Snake;
struct Field
{
public:
	Field();
	void init(const Snake* snake);
	void move();
	unsigned fieldSize() const;
	unsigned fieldWidth() const;
	unsigned fieldHeight() const;
	field_elem getElem(Index idx) const;
	bool isFood(Index idx) const;
	bool isEmpty(Index idx) const;
	void moveSnake(const Snake* snake);		// draw new snake location of field
	void growSnake(const Snake* snake);
	void display() const;

private:
	Index foodGen();			// generate new food index and set field[new index] to be the food


/*	static const unsigned WIDTH = 22;
	static const unsigned HEIGHT = 12;*/
	field_elem field[FIELD_SIZE];
	bool hasBeenEaten;
	Index foodIdx;

};



#endif /* FIELD_H_ */

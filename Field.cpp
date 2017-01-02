/*
 * Field.cpp
 *
 *  Created on: Dec 31, 2016
 *      Author: Apple
 */
#include "Field.h"
#include "Snake.h"
#include "Utils.h"
#include <random>

Field::Field(const Snake* snake)
{
	hasBeenEaten = false;
	foodIdx = foodGen();
	for(Index i=0; i!=FIELD_SIZE; i++) {
		if(i < WIDTH || i > FIELD_SIZE-WIDTH-1 || i % WIDTH == 0 || i % WIDTH == WIDTH-1)
			field[i] = WALL;
		else
			field[i] = EMPTY_SLOT;
	}
}

unsigned Field::fieldSize() const
{
	return FIELD_SIZE;
}

unsigned Field::fieldWidth() const
{
	return WIDTH;
}

unsigned Field::fieldHeight() const
{
	return HEIGHT;
}

field_elem Field::getElem(Index idx) const
{
	return field[idx];
}

bool Field::isFood(Index idx) const
{
		return idx == foodIdx;
}

bool Field::isEmpty(Index idx) const
{
	return field[idx] == EMPTY_SLOT;
}

void Field::moveSnake(const Snake* snake)
{
	Index old_tail = snake->getTailIdx();
	Index old_head = snake->getHeadIdx();
	Direc dir = snake->getCurrDir();

	if(field[old_head+dir] == EMPTY_SLOT || field[old_tail] == SNAKE_BODY) {
		field[old_tail]=EMPTY_SLOT;
		field[old_head+dir]=SNAKE_BODY;
	}
	else
		unix_error("Move to impossible place!");
}

void Field::growSnake(const Snake* snake)
{
	Index old_head = snake->getHeadIdx();
	Direc dir = snake->getCurrDir();

	if(old_head+dir == foodIdx && hasBeenEaten == false) {
		hasBeenEaten = true;
		field[foodIdx]=SNAKE_BODY;
	}
	else
		unix_error("Grow without food!");
}

Index Field::foodGen()
{
	if(hasBeenEaten == false) {
		if(field[foodIdx] == FOOD)
			return foodIdx;
		else
			unix_error("Food position inconsistent");
	}

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, FIELD_SIZE-1);

	Index i;
	do {
		i = distribution(generator);
	} while(field[i] != EMPTY_SLOT);

	return i;
}

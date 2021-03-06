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
#include <vector>
using std::cout;
using std::endl;
using std::vector;

Field::Field()
{
	num_empty=0;
	hasWin = false;
	hasBeenEaten = true;
	foodIdx = 0;
	for(Index i=0; i!=FIELD_SIZE; i++) {
		if(i < WIDTH || i > FIELD_SIZE-WIDTH-1 || i % WIDTH == 0 || i % WIDTH == WIDTH-1)
			field[i] = WALL;
		else {
			field[i] = EMPTY_SLOT;
			num_empty++;
		}
	}
}

void Field::init(const Snake* snake)
{
	hasBeenEaten = true;
	field[foodIdx = foodGen()] = FOOD;

	Index body = snake->getHeadIdx();
	unsigned cnt=0;
	while(body != snake->getTailIdx()) {
		field[body] = SNAKE_BODY;
		body += -snake->getCurrDir();
		cnt++;
	}

	if(cnt != snake->getLength()-1)
		unix_error("Initialization failed due to unconsistent length");
	field[body] = SNAKE_BODY;
	num_empty -= cnt+1;
}

void Field::move()
{
	if(hasBeenEaten)
		foodIdx = foodGen();

	if(foodIdx == -1) {
		hasWin = true;
		return;
	}

	field[foodIdx] = FOOD;
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

Index Field::getFoodIdx() const
{
	return foodIdx;
}

bool Field::isFood(Index idx) const
{
		return idx == foodIdx;
}

bool Field::isEmpty(Index idx) const
{
	return field[idx] == EMPTY_SLOT;
}

bool Field::isWall(Index idx) const
{
	return field[idx] == WALL;
}

bool Field::isSnake(Index idx) const
{
	return field[idx] == SNAKE_BODY;
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
		num_empty--;
	}
	else
		unix_error("Grow without food!");
}

vector<Index> idxRecord;
Index Field::foodGen()
{
	if(num_empty == 0) {
		hasWin = true;
		return -1;
	}
	if(hasBeenEaten == false) {
		if(field[foodIdx] == FOOD) {
			return foodIdx;
		}
		else
			unix_error("Food position inconsistent");
	}

	std::default_random_engine generator;
	std::random_device rd;
	std::uniform_int_distribution<int> distribution(0, FIELD_SIZE-1);

	Index i;
	do {
		i = distribution(rd);
	} while(field[i] != EMPTY_SLOT);

	idxRecord.push_back(i);
	hasBeenEaten = false;
	return i;
}

void Field::display(const Snake& s) const
{
	for(Index idx=0; idx != FIELD_SIZE; idx++) {

		if(idx % WIDTH == 0)
			cout << endl;

		if(idx == s.getHeadIdx()) {
			Direc dir = s.getCurrDir();
			switch(dir) {
			case UP: cout << '^';
			break;
			case DOWN: cout << 'v';
			break;
			case LEFT: cout << "<";
			break;
			case RIGHT: cout << '>';
			break;
			}
		}
		else if(idx == s.getTailIdx()) {
			cout << 'o';
		}
		else
			cout << field[idx];
	}
	cout << endl;
}

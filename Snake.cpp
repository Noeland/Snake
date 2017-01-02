/*
 * Snake.cpp
 *
 *  Created on: Dec 31, 2016
 *      Author: Apple
 */
#include "Snake.h"
#include "Field.h"
#include <list>
#include <iostream>
#include "Utils.h"
using namespace std;

const Direc INIT_DIR = RIGHT;
const unsigned INIT_LEN = 2;
const Index INIT_IDX = getIdx(1,1);

Snake::Snake(Field *f)
{
	currDir = INIT_DIR;
	isDead = false;
	field = f;
	length = INIT_LEN;
}

Snake::Snake(const Snake& other)
{
	currDir = other.currDir;
	isDead = other.isDead;
	field = other.field;
	length = other.length;
}

void Snake::moveTo()
{
	Index newHead = getHeadIdx() + currDir;

	if(field->isFood(newHead))
		return grow();

	if(!snake.empty()) {
		field->moveSnake(this);		// tell the field to show the corresponding change
		popTail();					// erase the old tail
		insertHead(newHead);		// insert new head
	}
	else
		unix_error("Snake has empty body.");
}

void Snake::grow()
{
	Index newHead = getHeadIdx() + currDir;

	if(!snake.empty()) {
		field->growSnake(this);
		insertHead(newHead);
		length++;
	}
}

bool Snake::isDeadMove(Direc Dir) const
{
	Index newHead = getHeadIdx() + Dir;
	return !field->isEmpty(newHead) && newHead != getTailIdx() && !field->isFood(newHead);
}

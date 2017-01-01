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

void Snake::moveTo()
{
	Index newHead = getHeadIdx() + currDir;

	if(field->isFood(newHead))
		return grow();

	if(!snake.empty()) {
		snake.pop_front();			// erase the old tail
		snake.push_back(newHead);	// insert new head
		field->moveSnake(this);		// tell the field to show the corresponding change
	}
	else
		unix_error("Snake has empty body.");
}

void Snake::grow()
{
	Index newHead = getHeadIdx() + currDir;

	if(!snake.empty()) {
		snake.push_back(newHead);
		field->growSnake(this);
		length++;
	}
}

bool Snake::isDeadMove(Direc Dir)
{
	Index newHead = getHeadIdx() + Dir;
	field_elem newpos = field->getElem(newHead);
	return newpos != EMPTY_SLOT && newHead != getTailIdx() && newpos != FOOD;
}

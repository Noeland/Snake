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
#include <queue>
#include <climits>
#include "Utils.h"
using namespace std;

const Direc INIT_DIR = RIGHT;
const unsigned INIT_LEN = 2;
const Index INIT_IDX = getIdx(1,1);

inline void printMap(bool arr[])
{
	unsigned size = WIDTH * HEIGHT;
	for(unsigned i=0; i!=size; i++) {
		if(i%WIDTH == 0)
			std::cout << std::endl;
		std::cout << arr[i] << ' ' ;
	}
	std::cout << std::endl;
}

inline void printMap(unsigned arr[])
{
	unsigned size = WIDTH * HEIGHT;
	for(unsigned i=0; i!=size; i++) {
		if(i%WIDTH == 0)
			std::cout << std::endl;
		std::cout << arr[i] << ' ' ;
	}
	std::cout << std::endl;
}

Snake::Snake(Field *f)
{
	currDir = INIT_DIR;
	isDead = false;
	field = f;
	length = INIT_LEN;

	unsigned l=0;
	while(l < length) {
		Index idx = INIT_IDX + (l++ * INIT_DIR);
		if(field->isEmpty(idx))
			insertHead(idx);
		else
			unix_error("Snake initilization failed: out of bound");
	}
}

Snake::Snake(const Snake& other)
{
	currDir = other.currDir;
	isDead = other.isDead;
	field = other.field;
	length = other.length;
	snake = other.snake;
}

void Snake::moveTo()
{
	if(isDeadMove(currDir)) {
		isDead = true;
		return;
	}
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

static Direc Direction[4] = {UP, DOWN, LEFT, RIGHT};
void Snake::move()
{
	unsigned len[5];
	Direc dir;
	for(int i=0; i!=4; i++) {
		switch(i) {
		case 0: dir = UP;
		break;
		case 1: dir = DOWN;
		break;
		case 2: dir = LEFT;
		break;
		case 3: dir = RIGHT;
		break;
		}
		Index head = getHeadIdx();
		len[i] = BFS(head+dir, field->getFoodIdx());
	}

	unsigned min = 4;
	len[min] = UINT_MAX;
	for(int i=0;i!=4;i++) {
		if(len[i] < len[min] && len[i] > 0)
			min=i;
	}

	if(len[min] != 0 && min != 4)
		currDir = Direction[min];
	else {
		cout << "impossible" << endl;
		isDead = true;
	}

	moveTo();
}

static bool debugFlag = false;
unsigned Snake::BFS(Index start, Index end)
{
	Direc dir = start - getHeadIdx();
	if(dir == -getCurrDir())
		return 0;

	if(isDeadMove(dir))
		return 0;

	unsigned size = field->fieldSize();

	bool explored[size];
	unsigned lenMap[size];
	memset(explored, 0, size * sizeof(bool));
	memset(lenMap, 0, size * sizeof(unsigned));
	lenMap[start] = 1;
	explored[start] = true;

//	for(auto i : snake)
//		explored[i] = true;

	queue<Index> q;
	q.push(start);
	Index idx = start;

	while(!q.empty()) {
		idx = q.front();
		q.pop();

		if(field->isFood(idx)) {
			if(lenMap[idx] == 0)
				unix_error("Unknown error in BFS");
			return lenMap[idx];
		}

		for(int i=0; i!=4; i++) {
			Direc dir;
			switch(i) {
			case 0: dir = UP;
				break;
			case 1: dir = DOWN;
				break;
			case 2: dir = LEFT;
				break;
			case 3: dir = RIGHT;
				break;
			}
			Index newIdx = idx+dir;
			bool safeCross = true;

			if(field->isSnake(newIdx)) {
				safeCross = false;
				int len = lenMap[idx]+1;
				int count = 0;
				for(auto index : snake) {
					if(count < len && index == newIdx) {
						safeCross = true;
						break;
					}
					else if(count >= len)
						break;
					count++;
				}
			}

			if( !explored[idx+dir] && !field->isWall(idx+dir) && safeCross) {
				q.push(idx+dir);
				lenMap[idx+dir] = lenMap[idx]+1;
				explored[idx+dir] = true;
				if(debugFlag == true)
					printMap(lenMap);
			}
		}

	}

	return 0;

}

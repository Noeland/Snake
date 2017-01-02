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
#include <stack>
#include <climits>
#include <cstring>
#include <curses.h>
#include <ncurses.h>
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
	path = other.path;
}

void Snake::moveTo(bool isVirtual)
{
	if(!path.empty()) {
		currDir = path.top();
		path.pop();
	}
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

void Snake::grow(bool isVirtual)
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

void Snake::move()
{
	if(!path.empty()) {
		moveTo();
		return;
	}

	BFS(getHeadIdx(), field->getFoodIdx(), path);

	moveTo();
}

static bool debugFlag = false;
unsigned Snake::BFS(Index start, Index end, stack<Direc>& Path)
{
	unsigned size = field->fieldSize();

	bool explored[size];
	unsigned lenMap[size];
	Direc dirMap[size];
	memset(explored, 0, size * sizeof(bool));
	memset(lenMap, 0, size * sizeof(unsigned));
	memset(dirMap, 0 , size * sizeof(Direc));
	lenMap[start] = 0;
	explored[start] = true;

//	for(auto i : snake)
//		explored[i] = true;

	queue<Index> q;
	q.push(start);
	Index idx = start;

	while(!q.empty()) {
		idx = q.front();
		q.pop();

		if(idx == end) {
			while(idx != start) {
				Path.push(-dirMap[idx]);
				idx += dirMap[idx];
			}
			return lenMap[end];
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
				dirMap[idx+dir] = -dir;
				if(debugFlag == true)
					printMap(lenMap);
			}
		}

	}

	return 0;

}

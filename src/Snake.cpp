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

Snake& Snake::operator=(const Snake& other)
{
	currDir = other.currDir;
	isDead = other.isDead;
	field = other.field;
	length = other.length;
	snake = other.snake;
	path = other.path;

	return *this;
}

void Snake::moveTo(bool isVirtual)
{
	if(isVirtual == true) {
		if(!path.empty()) {
			currDir = path.top();
			path.pop();
		}

		Index newHead = getHeadIdx() + currDir;

		if(field->isFood(newHead))
			return grow(isVirtual);

		if(!snake.empty()) {
			popTail();					// erase the old tail
			insertHead(newHead);		// insert new head
		}
		else
			unix_error("Virtual Snake has empty body.");
		return;
	}

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
		if(isVirtual == false)		// sync the change to field only if isVirtual == false
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

	// Virtual
	Snake backup_snake = *this;
	BFS(getHeadIdx(), field->getFoodIdx(), path);

	if(!path.empty()) {
		stack<Direc> backup_path = path;

		unsigned len = length;
		do {
			moveTo(true);
		} while(!path.empty() && len == length);

		BFS(getHeadIdx(), getTailIdx(), path);
		bool solved = !path.empty();

		if(solved) {
			*this = backup_snake;
			path = backup_path;
			moveTo();
			return;
		}
		else {
			*this = backup_snake;
			BFS(getHeadIdx(), getTailIdx(), path);
			moveTo();
			return;
		}
	}


	moveTo();
}

static bool debugFlag = false;
unsigned Snake::BFS(Index start, Index end, stack<Direc>& Path)
{
	unsigned size = field->fieldSize();

	bool explored[size];
	unsigned lenMap[size];
	Direc dirMap[size];
	bool isBody[size];
	memset(explored, 0, size * sizeof(bool));
	memset(lenMap, 0, size * sizeof(unsigned));
	memset(dirMap, 0 , size * sizeof(Direc));
	memset(isBody, 0, size*sizeof(bool));
	lenMap[start] = 0;
	explored[start] = true;

	for(auto i : snake)
		isBody[i] = true;

	queue<Index> q;
	q.push(start);
	Index idx = start;

	while(!q.empty()) {
		idx = q.front();
		q.pop();

		if(idx == end) {
			stack<Direc> path;
			while(idx != start) {
				path.push(-dirMap[idx]);
				idx += dirMap[idx];
			}
			if(end == field->getFoodIdx())
				Path = path;
			else
				Path.push(path.top());

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

			if(isBody[newIdx]) {

				if(length != 2) {
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
				else
				  safeCross = false;
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

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
#include <cstdlib>
#include "Utils.h"
using std::vector;
using std::stack;
using std::queue;

const Direc INIT_DIR = RIGHT;
const unsigned INIT_LEN = 5;
const Index INIT_IDX = getIdx(1,1);

inline void printMap(bool arr[])
{
	unsigned size = WIDTH * HEIGHT;
	for(unsigned i=0; i!=size; i++) {
		if(i%WIDTH == 0)
			std::cout << std::endl;
		std::cout.width(4); std::cout << std::right << arr[i];
	}
	std::cout << std::endl;
}

inline void printMap(unsigned arr[])
{
	unsigned size = WIDTH * HEIGHT;
	for(unsigned i=0; i!=size; i++) {
		if(i%WIDTH == 0)
			std::cout << std::endl;
		if(arr[i] != 0) {
			std::cout << "\033[1;31m";
			std::cout.width(4); std::cout << std::right << arr[i] << "\033[0m";
		}
		else {
			std::cout.width(4); std::cout << std::right << 0;
		}
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
	myBFS(getHeadIdx(), field->getFoodIdx(), &path);

	if(!path.empty()) {
		stack<Direc> backup_path = path;

		unsigned len = length;
		do {
			moveTo(true);
		} while(!path.empty() && len == length);

		field->turnOffFood();
		bool solved = DFS(getHeadIdx(), getTailIdx(), nullptr) != 0;
		field->turnOnFood();
//		bool solved = !path.empty();

		if(solved) {
			*this = backup_snake;
			currDir = backup_path.top();
			moveTo();
			return;
		}
		else {
			*this = backup_snake;

			return chaseTail();
		}
	}

	return chaseTail();
//	moveTo();
}

void Snake::chaseTail()
{
	Direc directions[4] = {UP, DOWN, LEFT, RIGHT};
	stack<Direc> Paths[4];
	unsigned len[5];
	for(int i = 0; i != 4; i++)
		len[i] = DFS(getHeadIdx()+directions[i], getTailIdx(), nullptr);

	unsigned max_idx = 4;
	len[max_idx] = 0;
	for(int i=0; i!=4; i++) {
		if(len[i] > len[max_idx])
			max_idx = i;
	}

	if(len[max_idx] != 0) {
		path = Paths[max_idx];
		path.push(directions[max_idx]);
	}
	else {

		field->turnOffFood();
		for(int i = 0; i != 4; i++)
			len[i] = DFS(getHeadIdx()+directions[i], field->getFoodIdx(), nullptr);
		field->turnOnFood();

		max_idx=4;
		len[max_idx] = 0;
		for(int i=0; i!=4; i++) {
			if(len[i] > len[max_idx])
				max_idx = i;
		}

		if(len[max_idx] !=0) {
			path = Paths[max_idx];
			path.push(directions[max_idx]);
		}
		else
			path.push(currDir);
	}

	moveTo();
}

static bool debugFlag = false;
unsigned Snake::myBFS(Index start, Index end, std::stack<Direc> *Path)
{
	if(start-getHeadIdx() == - currDir || (start-getHeadIdx() != 0 && isDeadMove(start-getHeadIdx())))
		return 0;
	if(Path != nullptr && !Path->empty())
		unix_error("Get non-empty path stack!");

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
			while(idx != start && Path!=nullptr) {
				Path->push(-dirMap[idx]);
				idx += dirMap[idx];
			}
//			if(!path.empty())
//				Path.push(path.top());

			return lenMap[end];
		}

		for(int i=0; i!=4; i++) {
			Direc dir;
			switch(i) {
			case 0: dir = currDir;
				break;
			case 1: dir = -currDir;
				break;
			case 2: dir = (currDir == UP || currDir == DOWN ) ? RIGHT : UP;
				break;
			case 3: dir = (currDir == UP || currDir == DOWN ) ? LEFT : DOWN;
				break;
			}
			Index newIdx = idx+dir;
			bool safeCross = true;
			if(isBody[newIdx] && newIdx != getTailIdx())
				safeCross = false;

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

unsigned Snake::DFS(Index start, Index end, std::stack<Direc> *Path)
{

	if(start-getHeadIdx() == -currDir)
		return 0;

	if(isDeadMove(start-getHeadIdx()))
		return 0;

	if(field->isFood(start))
		return 0;

	if(Path != nullptr && !Path->empty())
		unix_error("Non-empeth path stack in DFS!");

	unsigned size = field->fieldSize();

	bool explored[size];
	unsigned lenMap[size];
	Direc dirMap[size];
	bool isBody[size];
	memset(explored, 0, size * sizeof(bool));
	memset(lenMap, 0, size * sizeof(unsigned));
	memset(dirMap, 0 , size * sizeof(Direc));
	memset(isBody, 0, size*sizeof(bool));
	lenMap[start] = 1;
	explored[start] = true;

	for(auto i : snake)
		isBody[i] = true;

	stack<Index> q;
	q.push(start);
	Index idx = start;

	while(!q.empty()) {
		idx = q.top();
		q.pop();

		if(idx == end) {

			while(idx != start && Path != nullptr) {
				Path->push(-dirMap[idx]);
				idx += dirMap[idx];
			}
			return lenMap[end];
		}

		Direc dir;
		int manhdist[5];
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
			if(!isBody[idx+dir] && !field->isWall(idx+dir) && !explored[idx+dir])
				manhdist[i] = abs( (idx+dir)/WIDTH - end/WIDTH) + abs((idx+dir) % WIDTH - end % WIDTH);
			else
				manhdist[i] = 0;
		}

		int max_idx = 4, secmax=4, thirdmax=4, last=4;
		manhdist[max_idx] = 0;
		for(int i=0; i!=4; i++) {
			if(manhdist[i] >= manhdist[max_idx]) {
				last = thirdmax;
				thirdmax = secmax;
				secmax = max_idx;
				max_idx = i;
			}
			else if(manhdist[i] >= manhdist[secmax]) {
				last = thirdmax;
				thirdmax = secmax;
				secmax = i;
			}
			else if(manhdist[i] >= manhdist[thirdmax]) {
				last = thirdmax;
				thirdmax = i;
			}
			else {
				last = i;
			}
		}

		Direc directions[4] = {UP, DOWN, LEFT, RIGHT};
		Direc dir1, dir2, dir3, dir4;
		if(manhdist[max_idx] != manhdist[secmax]) {
			dir1 = directions[max_idx];
			dir2 = directions[secmax];
			dir3 = directions[thirdmax];
			dir4 = directions[last];
		}
		else if(manhdist[secmax] != manhdist[thirdmax]) {
			if(directions[secmax]==currDir) {
				dir1 = currDir;
				dir2 = directions[max_idx];
				dir3 = directions[thirdmax];
				dir4 = directions[last];
			}
			else {
				dir1 = directions[max_idx];
				dir2 = directions[secmax];
				dir3 = directions[thirdmax];
				dir4 = directions[last];
			}
		}
		else if(manhdist[thirdmax] != manhdist[last]) {
			if(directions[secmax]==currDir || directions[thirdmax] == currDir) {
				dir1 = currDir;
				dir2 = directions[max_idx];
				dir3 = directions[thirdmax] == currDir ? directions[secmax] : directions[thirdmax];
				dir4 = directions[last];
			}
			else {
				dir1 = directions[max_idx];
				dir2 = directions[secmax];
				dir3 = directions[thirdmax];
				dir4 = directions[last];
			}
		}
		else if(manhdist[max_idx] != 0){
			dir1 = directions[max_idx];
			dir2 = directions[secmax];
			dir3 = directions[thirdmax];
			dir4 = directions[last];
		}
		else {
			dir1 = currDir;
			dir2 = -currDir;
			dir3 = (currDir == UP || currDir == DOWN) ? RIGHT : UP;
			dir4 =  (currDir == UP || currDir == DOWN) ?  LEFT : DOWN;
		}


		for(int i=0; i!=4; i++) {
			switch(i) {
			case 3: dir = dir1;
			break;
			case 2: dir = dir2;
			break;
			case 1: dir = dir3;
			break;
			case 0: dir = dir4;
			break;
			}

			Index newIdx = idx+dir;
			bool safeCross = true;
			if(isBody[newIdx] && newIdx != getTailIdx())
				safeCross = false;

			if( !explored[idx+dir] && !field->isWall(idx+dir) && safeCross && !field->isFood(idx+dir)) {
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

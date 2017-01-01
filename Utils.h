/*
 * Util.h
 *
 *  Created on: Dec 31, 2016
 *      Author: Apple
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <string>
#include <cstdlib>
#include <iostream>

typedef char field_elem;
typedef int Direc;
typedef unsigned Index;

const unsigned WIDTH = 22;
const unsigned HEIGHT = 12;

#define getIdx(i,j) i*WIDTH + j


void unix_error(const std::string& msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}



#endif /* UTILS_H_ */

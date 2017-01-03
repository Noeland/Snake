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

const unsigned WIDTH = 12;
const unsigned HEIGHT = 7;

#define getIdx(i,j) i*WIDTH + j


inline void unix_error(const std::string& msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}



#endif /* UTILS_H_ */

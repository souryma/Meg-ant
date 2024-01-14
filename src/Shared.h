#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <list>
#include "Square.h"

template<typename T>
using InternalArray = std::vector<T>;

template<typename T>
using InternalArray2D = InternalArray<InternalArray<T>>;

template<typename T>
using InternalList = std::list<T>;


////////////////////////////////////////////////////
// 
// An accurate way to test speed.
// 
////////////////////////////////////////////////////

using namespace std::chrono;

class InternalTimer
{
private:
	time_point<high_resolution_clock> t1;

public:
	void Start()
	{
		t1 = high_resolution_clock::now();
	}

	float Stop()
	{
		auto t2 = high_resolution_clock::now();
		return (float)duration_cast<milliseconds>(t2 - t1).count();
	}
};

struct SMission
{
	CSquare CurrentSquare;
	CSquare TargetSquare;
	int LastFrameNumber; // or the last turn id
	bool HasToRemove; // indicate if a mission is canceled or done
};

enum EDirection
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};
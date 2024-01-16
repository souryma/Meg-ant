#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <set>
#include "Square.h"

#define MY_PLAYER_ID 0
#define MY_ANT_CLOSE_DIST 5
#define ENEMY_CLOSE_DIST 8
#define ENEMY_CLOSE_DIST2 (ENEMY_CLOSE_DIST * ENEMY_CLOSE_DIST)
#define ENEMY_CLOSER_DIST 5
#define ENEMY_CLOSEST_DIST 2
#define FOOD_CLOSE_DIST 2
#define AREA_DIAMETER 10
#define NEIGHBOR_COUNT 4

using namespace std;

template<typename T>
using InternalArray = std::vector<T>;

template<typename T>
using InternalArray2D = InternalArray<InternalArray<T>>;

template<typename T>
using InternalList = std::list<T>;

template<typename T>
using InternalQueue = std::queue<T>;

template<typename T>
using InternalSet = std::set<T>;

template<typename T, typename U>
using InternalMap = std::unordered_map<T, U>;

////////////////////////////////////////////////////
// 
// An accurate way to test speed.
// 
////////////////////////////////////////////////////

using namespace std::chrono;

class CCustomTimer
{
private:
	time_point<high_resolution_clock> _t1;

public:
	void Start()
	{
		_t1 = high_resolution_clock::now();
	}

	float Stop()
	{
		auto t2 = high_resolution_clock::now();
		return (float)duration_cast<milliseconds>(t2 - _t1).count();
	}
};

struct SMission
{
	CSquare CurrentSquare;
	CSquare TargetSquare;
	int LastFrameNumber; // or the last turn id
	bool HasToRemove; // indicate if a mission is canceled or done
};

struct SFightArea
{
	CSquare* MyHill;
	bool IsChecked;
	bool IsAreaContainsMyHill;
	int Player;

	InternalArray<CAnt*> Ants;
	InternalArray<CSquare*> Squares;
	InternalArray<CSquare*> Edges;
};

enum EDirection
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct SGlobal
{
	static int Rows;
	static int Cols;
	static InternalArray2D<CSquare*> Grid;
};

class CUtilityFunctions
{
public:
	static int DistRow(const CSquare& S1, const CSquare& S2)
	{
		int dRow = std::abs(S1.Row - S2.Row);
		return min(dRow, SGlobal::Rows - dRow);
	};

	static int DistCol(const CSquare& S1, const CSquare& S2)
	{
		int dCol = std::abs(S1.Col - S2.Col);
		return min(dCol, SGlobal::Cols - dCol);
	};

	static bool IsTooCloseTo(const CSquare& S1, const CSquare& S2)
	{
		int dRow = DistRow(S1, S2);
		int dCol = DistCol(S1, S2);

		return (dCol <= 1 && dRow <= ENEMY_CLOSEST_DIST) || (dCol == ENEMY_CLOSEST_DIST && dRow <= 1);
	};

	static bool IsCloseTo(const CSquare& S1, const CSquare& S2)
	{
		int dRow = DistRow(S1, S2);
		int dCol = DistCol(S1, S2);

		if (dRow + dCol <= ENEMY_CLOSE_DIST)
		{
			return (dRow != 0 && dCol != 0);
		}

		return false;
	};

	static int Dist(CSquare& S1, CSquare& S2)
	{
		int dRow = std::abs(S1.Row - S2.Row);
		int dCol = std::abs(S1.Col - S2.Col);
		return min(dRow, SGlobal::Rows - dRow) + min(dCol, SGlobal::Cols - dCol);
	};

	static int GetDirectionFromMovement(CSquare& From, CSquare& To)
	{
		if (To.Row - From.Row == 0)
		{
			return To.Col - From.Col > 0 ? 1 : 3;
		}

		if (To.Col - From.Col == 0)
		{
			return To.Row - From.Row > 0 ? 2 : 0;
		}

		return 0;
	};
};
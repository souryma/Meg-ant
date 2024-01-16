#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.h"
#include "Bug.h"
#include "Square.h"
#include "Shared.h"
#include "Ant.h"
#include "Location.h"

/*
	constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = { 'N', 'E', 'S', 'W' };
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };      //{N, E, S, W}

/*
	struct to store current state information
*/
class CState
{
private:
	bool _GameOver;
	int64_t _Seed;

public:
	int Rows, Cols;
	int Turn, TotalTurns;
	int NumberOfPlayers;

	double AttackRadius, SpawnRadius, ViewRadius;

	double LoadTime, TurnTime;
	InternalArray<double> Scores;
	//InternalArray<Location> myAnts, enemyAnts, myHills, enemyHills, food, exploringAnts, defendinAnts, targetedFoods;

	InternalArray<CSquare*> Foods;
	InternalArray<CAnt*> MyAnts, EnemyAnts;
	InternalArray<CSquare*> MyHills, EnemyHills;

	///================ DEBUG
	CBug Bug;
	InternalTimer Timer;

public:
	CState();
	~CState();

	void Setup();
	void Reset();

	void MakeMove(SLocation& Loc, int Direction);

	double Distance(const SLocation& Loc1, const SLocation& Loc2);
	void SetLocation(SLocation& IOLocation, int Direction);

	void UpdateVisionInformation();

	friend std::ostream& operator<<(std::ostream& os, const CState& state);
	friend std::istream& operator>>(std::istream& is, CState& state);
};


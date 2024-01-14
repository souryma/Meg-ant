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
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
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

    InternalArray2D<CSquare> Grid;
    //InternalArray<Location> myAnts, enemyAnts, myHills, enemyHills, food, exploringAnts, defendinAnts, targetedFoods;

    InternalArray<CSquare> Foods;
    InternalArray<CAnt> MyAnts, EnemyAnts;
    InternalArray<CSquare> MyHills, EnemyHills;

    ///================ DEBUG
    Bug bug;
    Timer timer;

public:
    CState();
    ~CState();

    void Setup();
    void Reset();

    void makeMove(const SLocation &loc, int direction);

    double Distance(const SLocation &loc1, const SLocation &loc2);
    SLocation GetLocation(const SLocation &startLoc, int direction);

    void UpdateVisionInformation();

    friend std::ostream& operator<<(std::ostream &os, const CState &state);
    friend std::istream& operator>>(std::istream &is, CState &state);
};


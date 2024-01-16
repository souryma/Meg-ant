#pragma once

#include "Shared.h"
#include "Location.h"

// Forward declarations
class CSquare;

class CAnt
{
private:
	static int MyAntIDCounter;
	bool _DancingState = false;
	EDirection _PreviousDirection = NORTH;

public:
	int ID;
	SLocation Location;
	int EnemyCount;
	int AlliesCount;
	bool IsEnemy;
	// True if the ant has moved this turn
	bool HasMoved;

	CSquare* SquarePtr;
	SMission* MissionPtr;
	InternalArray<CAnt*> CloseEnemies;
	InternalArray<CAnt*> CloserEnemies;
	InternalArray<CAnt*> ClosestEnemies;
	bool IsInMyArea;
	bool HasMission;

	bool IsHill;
	bool IsMyHill;

public:
	CAnt(int InRow, int InCol);
	CAnt(CSquare* square);

	// Make the ant go in random directions
	EDirection Explore();

	// Make the ant switch between two squares
	EDirection Dance(EDirection direction);
};


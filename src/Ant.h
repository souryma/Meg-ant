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
	bool IsAttacking;
	bool IsDefending;
	bool IsExploring;
	bool IsDancing;
	bool IsInDanger;
	bool IsReallyInDanger;
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
	CAnt(SLocation location);
	CAnt(int InRow, int InCol);
	CAnt(CSquare* square);

	void Attack();
	void Defend();
	EDirection Explore(CSquare square);

	// Make the ant switch between two squares
	EDirection Dance(EDirection direction);
};


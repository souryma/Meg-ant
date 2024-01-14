#pragma once

#include "Shared.h"
#include "Location.h"

// Forward declarations
class CSquare;

class CAnt
{
private:
	bool _DancingState = false;
	EDirection _PreviousDirection = NORTH;

public:
	SLocation Location;
	bool IsAttacking;
	bool IsDefending;
	bool IsExploring;
	bool IsDancing;
	bool IsInDanger;
	bool IsReallyInDanger;
	int EnemyCount;
	// True if the ant has moved this turn
	bool HasMoved;

	CSquare* SquarePtr;
	SMission* Mission;
	bool HasMission;

public:
	CAnt(SLocation location);
	CAnt(int InRow, int InCol);

	void Attack();
	void Defend();
	void Explore();

	// Make the ant switch between two squares
	EDirection Dance(EDirection direction);
};


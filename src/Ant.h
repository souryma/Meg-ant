#pragma once
#include "Location.h"
#include "Direction.h"

class Ant
{
private:
	bool _DancingState = false;
	EDirection _PreviousDirection = NORTH;

public:
	Location Location;
	bool IsAttacking;
	bool IsDefending;
	bool IsExploring;
	bool IsDancing;
	bool IsInDanger;
	bool IsReallyInDanger;
	int EnemyCount;
	// True if the ant has moved this turn
	bool HasMoved;

public:
	Ant();
	void Attack();
	void Defend();
	void Explore();
	// Make the ant switch between two squares
	EDirection Dance(EDirection direction);
};


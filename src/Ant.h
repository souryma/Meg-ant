#pragma once

#include "Location.h"
#include "Direction.h"

class Ant
{
private:
	bool dancingState = false;
	EDirection previousDirection = NORTH;

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
	EDirection Danse(EDirection direction);
};


#include "Ant.h"

Ant::Ant(Location location) {
	IsAttacking = false;
	IsDefending = false;
	IsExploring = false;
	IsDancing = false;

	IsInDanger = false;
	IsReallyInDanger = false;
	EnemyCount = 0;
	HasMoved = false;
	AntLocation = location;
}

// Make the ant switch between two squares in the desired direction
EDirection Ant::Dance(EDirection direction)
{
	IsDancing = true;

	// If the direction of dance is different than before, we reset dancing state
	if (direction != _PreviousDirection)
	{
		_DancingState = true;
	}

	// If dancing state is true, the ant must dance in the direction, if false, opposite direction
	switch (direction) {
	case NORTH:
		direction = _DancingState ? direction : SOUTH;
		break;
	case EAST:
		direction = _DancingState ? direction : WEST;
		break;
	case SOUTH:
		direction = _DancingState ? direction : NORTH;
		break;
	case WEST:
		direction = _DancingState ? direction : EAST;
		break;
	}

	_DancingState = !_DancingState;
	HasMoved = true;
	_PreviousDirection = direction;

	return direction;
}
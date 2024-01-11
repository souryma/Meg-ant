#include "Ant.h"

Ant::Ant() {
	IsAttacking = false;
	IsDefending = false;
	IsExploring = false;
	IsDancing = false;

	IsInDanger = false;
	IsReallyInDanger = false;
	EnemyCount = 0;
}

// Make the ant switch between two squares in the desired direction
EDirection Ant::Danse(EDirection direction)
{
	IsDancing = true;

	if (direction != previousDirection)
	{
		dancingState = true;
	}

	switch (direction) {
	case 0: // N
		Location.col = dancingState ? Location.col - 1 : Location.col + 1;
		break;
	case 1: // E
		Location.col = dancingState ? Location.row + 1 : Location.row - 1;
		break;
	case 2: // S
		Location.col = dancingState ? Location.col + 1 : Location.col - 1;
		break;
	case 3: // W
		Location.col = dancingState ? Location.row - 1 : Location.row + 1;
		break;
	}
	dancingState = !dancingState;

	HasMoved = true;

	previousDirection = direction;
	return EAST;
}
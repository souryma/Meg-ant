#include "Ant.h"

// Static members
int CAnt::MyAntIDCounter = 0;

CAnt::CAnt(int InRow, int InCol) {
	MyAntIDCounter++;
	ID = MyAntIDCounter;

	IsInMyArea = false;

	EnemyCount = 0;
	AlliesCount = 0;
	HasMoved = false;
	Location.Row = InRow;
	Location.Col = InCol;

	SquarePtr = new CSquare(InRow, InCol);
	MissionPtr = nullptr;
	IsHill = false;
	IsMyHill = false;
}

CAnt::CAnt(CSquare* square)
{
	MyAntIDCounter++;
	ID = MyAntIDCounter;

	IsInMyArea = false;

	EnemyCount = 0;
	AlliesCount = 0;
	HasMoved = false;
	Location.Row = square->Row;
	Location.Col = square->Col;

	SquarePtr = square;
	MissionPtr = nullptr;
	IsHill = false;
	IsMyHill = false;
}

EDirection CAnt::Explore()
{
	HasMoved = true;
	return (EDirection)(rand() % 4);
}

// Returns the direction that will make the ant switch between two squares in the desired direction
EDirection CAnt::Dance(EDirection direction)
{
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



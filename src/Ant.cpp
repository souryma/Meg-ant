#include "Ant.h"

// Static members
int CAnt::MyAntIDCounter = 0;

CAnt::CAnt(int InRow, int InCol) {
	MyAntIDCounter++;
	ID = MyAntIDCounter;

	IsAttacking = false;
	IsDefending = false;
	IsExploring = true;
	IsDancing = false;
	IsInMyArea = false;

	IsInDanger = false;
	IsReallyInDanger = false;
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

	IsAttacking = false;
	IsDefending = false;
	IsExploring = true;
	IsDancing = false;
	IsInMyArea = false;

	IsInDanger = false;
	IsReallyInDanger = false;
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

// Se dirige toujours vers l'EST si le chemin est libre
EDirection CAnt::Explore(CSquare antSquare)
{
	CSquare* neighbours = antSquare.GetNeighbours();
	
	// turn left each turn if able

	 //Get the direction the ant was heading the turn before
	int neighbourDirection = PreviousDirection;

	// if all neighbours are available, go east
	/*if (neighbours[0].IsSafe && neighbours[1].IsSafe && neighbours[2].IsSafe && neighbours[3].IsSafe)
	{
		if (neighbourDirection == 3)
			neighbourDirection = 0;
		else
			neighbourDirection += 1;

		return (EDirection)neighbourDirection;
	}*/

	for (int i = 0; i < 4; i++) {
		if (neighbourDirection > 3)
			neighbourDirection = 0;
		else
			neighbourDirection += 1;

		if (neighbours[neighbourDirection].IsWater == false)
		{
			return (EDirection)neighbourDirection;
		}
	}

	HasMoved = true;
	return PreviousDirection;

	//if (antSquare.GetNeighbours()[0].IsSafe)
	//	return EAST;

	//if (antSquare.GetNeighbours()[1].IsSafe)
	//	return SOUTH;

	//if (antSquare.GetNeighbours()[2].IsSafe)
	//	return WEST;

	//if (antSquare.GetNeighbours()[3].IsSafe)
	//	return NORTH;

}

// Make the ant switch between two squares in the desired direction
EDirection CAnt::Dance(EDirection direction)
{
	IsDancing = true;

	// If the direction of dance is different than before, we reset dancing state
	if (direction != PreviousDirection)
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
	PreviousDirection = direction;

	return direction;
}



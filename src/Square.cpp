#include "Square.h"
#include "Shared.h"

// Static members
int CSquare::_SquareIDCounter = 0;

CSquare::CSquare()
{
	ID = ++_SquareIDCounter;

	IsVisible = IsWater = IsHill = IsFood = 0;
	IsSafe = true;
	AntPlayerId = HillPlayer = -1;
	IsEnemy = false;
	IsReached = false;

	ExplorationWeight = 100;

	// For A*
	Dist = 0;
	TotalCost = 0;
	Parent = nullptr;

	Previous = nullptr;
	FoodSourceFinding = nullptr;
}

CSquare::CSquare(int InRow, int InCol)
{
	CSquare();
	Row = InRow;
	Col = InCol;
}

void CSquare::Reset()
{
	IsVisible = 0;
	IsHill = 0;
	IsFood = 0;
	AntPlayerId = HillPlayer = -1;
}


//EDirection CSquare::GetDirectionTo(CSquare S)
//{
//    if (Row == S.Row)
//    {
//        if (Row + 1 == S.Row) return EDirection::EAST;
//        if (Row - 1 == S.Row) return EDirection::WEST;
//        return S.Row == 0 ? EDirection::EAST : EDirection::WEST;
//    }
//    else
//    {
//        if (Col + 1 == S.Col) return EDirection::NORTH;
//        if (Col - 1 == S.Col) return EDirection::SOUTH;
//        return S.Col == 0 ? EDirection::NORTH : EDirection::SOUTH;
//    }
//}

InternalArray<CSquare*> CSquare::GetNeighbors()
{
	InternalArray<CSquare*> neighbors;

	neighbors.push_back(SGlobal::Grid[(Row + 0 + SGlobal::Rows) % SGlobal::Rows][(Col + 1 + SGlobal::Cols) % SGlobal::Cols]);
	neighbors.push_back(SGlobal::Grid[(Row + 1 + SGlobal::Rows) % SGlobal::Rows][(Col + 0 + SGlobal::Cols) % SGlobal::Cols]);
	neighbors.push_back(SGlobal::Grid[(Row + 0 + SGlobal::Rows) % SGlobal::Rows][(Col - 1 + SGlobal::Cols) % SGlobal::Cols]);
	neighbors.push_back(SGlobal::Grid[(Row - 1 + SGlobal::Rows) % SGlobal::Rows][(Col + 0 + SGlobal::Cols) % SGlobal::Cols]);

	return neighbors;
}

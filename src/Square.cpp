#include "Square.h"
#include "Shared.h"

// Static members
int CSquare::SquareIDCounter = 0;

CSquare::CSquare()
{
    ID = ++SquareIDCounter;

	IsVisible = IsWater = IsHill = IsFood = 0;
    IsSafe = true;
	AntPlayerId = HillPlayer = -1;
    IsEnemy = false;
	IsReached = false;
	IsReachedByMyBot = false;

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

// TODO : Return properly
CSquare* CSquare::GetNeighbours()
{
    CSquare neighbours[4];
    
    neighbours[0] = CSquare(Row, Col+1);
    neighbours[1] = CSquare(Row+1, Col);
    neighbours[2] = CSquare(Row, Col-1);
    neighbours[3] = CSquare(Row-1, Col);

    return neighbours;
}

InternalArray<CSquare*> CSquare::GetNeighbors()
{
    InternalArray<CSquare*> neighbors;

    neighbors.push_back(SGlobal::Grid[(Row + 0 + SGlobal::Rows) % SGlobal::Rows][(Col + 1 + SGlobal::Cols) % SGlobal::Cols]);
    neighbors.push_back(SGlobal::Grid[(Row + 1 + SGlobal::Rows) % SGlobal::Rows][(Col + 0 + SGlobal::Cols) % SGlobal::Cols]);
    neighbors.push_back(SGlobal::Grid[(Row + 0 + SGlobal::Rows) % SGlobal::Rows][(Col - 1 + SGlobal::Cols) % SGlobal::Cols]);
    neighbors.push_back(SGlobal::Grid[(Row - 1 + SGlobal::Rows) % SGlobal::Rows][(Col + 0 + SGlobal::Cols) % SGlobal::Cols]);

    return neighbors;
}

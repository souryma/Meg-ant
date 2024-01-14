#include "Square.h"

CSquare::CSquare()
{
	IsVisible = IsWater = IsHill = IsFood = 0;
    IsSafe = true;
	ant = hillPlayer = -1;

	IsReached = false;
	IsReachedByMyBot = false;

    ExplorationWeight = 100;
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
    ant = hillPlayer = -1;
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

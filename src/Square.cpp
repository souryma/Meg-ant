#include "Square.h"

Square::Square()
{
	isVisible = isWater = isHill = isFood = 0;
	ant = hillPlayer = -1;

	IsReached = false;
	IsReachedByMyBot = false;

    ExplorationWeight = 100;
}

void Square::reset()
{
    isVisible = 0;
    isHill = 0;
    isFood = 0;
    ant = hillPlayer = -1;
    deadAnts.clear();
}


EDirection Square::GetDirectionTo(Square S)
{
    if (Row == S.Row)
    {
        if (Row + 1 == S.Row) return EDirection::EAST;
        if (Row - 1 == S.Row) return EDirection::WEST;
        return S.Row == 0 ? EDirection::EAST : EDirection::WEST;
    }
    else
    {
        if (Col + 1 == S.Col) return EDirection::NORTH;
        if (Col - 1 == S.Col) return EDirection::SOUTH;
        return S.Col == 0 ? EDirection::NORTH : EDirection::SOUTH;
    }
}
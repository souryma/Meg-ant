#include "Square.h"

Square::Square()
{
	isVisible = isWater = isHill = isFood = 0;
	ant = hillPlayer = -1;

	IsReached = false;
	IsReachedByMyBot = false;
}

void Square::reset()
{
    isVisible = 0;
    isHill = 0;
    isFood = 0;
    ant = hillPlayer = -1;
    deadAnts.clear();
}


EDirection GetDirectionTo(Square S)
{

}
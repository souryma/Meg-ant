#ifndef SQUARE_H_
#define SQUARE_H_

#include <vector>
#include "Direction.h"

/*
    struct for representing a square in the grid.
*/
struct Square
{
public:
    bool isVisible;
    bool isWater;
    bool isHill;
    bool isFood;

    int ant, hillPlayer;
    std::vector<int> deadAnts;

    //======================= FOR A*
    int F; // only for food

    //=======================
    bool IsTargeted;
    bool IsReached;
    bool IsReachedByMyBot;

public:
    Square();

    //resets the information for the square except water information
    void reset();

    EDirection GetDirectionTo(Square S);
};

#endif //SQUARE_H_

#pragma once

#include "State.h"

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves

    Location getClosestFood(Location ant); // returns the closest food location of an ant
    bool isFoodAlreadyTargeted(Location food); //returns true if a food is already targeted by an ant
};

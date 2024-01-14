#pragma once

#include "State.h"

#define MY_PLAYER_ID 0


/*
    This struct represents your bot in the game of Ants
*/
class CBot
{
public:
    CState State;
    InternalList<SMission> Missions;
    bool IsTimeout;

public:
    CBot();

    void PlayGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves

private:
    ///================ MISSIONS
    void _AssignMissions();
    void _CreateMissions();

    ///================ MISC
    void _AStar(CSquare& From, CSquare& To, bool FirstTree);

    // No enemy / My ant is there
    bool _IsSafe(CSquare& Dest);
};
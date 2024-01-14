#pragma once

#include <vector>

// Forward declarations
class CAnt;

/*
    struct for representing a square in the grid.
*/
class CSquare
{
public:
    bool IsVisible;
    bool IsWater;
    bool IsHill;
    bool IsFood;

    CAnt* AntPtr;

    int ant, hillPlayer;

    //======================= FOR A*
    int F; // only for food

    //=======================
    bool IsTargeted;
    bool IsReached;
    bool IsReachedByMyBot;

    int Row, Col;

    ///======================= EXPLORATION
    int ExplorationWeight;

public:
    CSquare();
    CSquare(int InRow, int InCol);

    void Reset();

    //EDirection GetDirectionTo(CSquare S);

    CSquare* GetNeighbours();

    bool operator==(const CSquare& Other) const
    {
        return (Row == Other.Row) && (Col == Other.Col);
    }
};

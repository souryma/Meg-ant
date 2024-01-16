#pragma once

#include <set>
#include <vector>

// Forward declarations
class CAnt;

template<typename T>
using InternalArray = std::vector<T>;

/*
    struct for representing a square in the grid.
*/
class CSquare
{
private:
    static int SquareIDCounter;

public:
    int ID;
    bool IsVisible;
    bool IsWater;
    bool IsHill;
    bool IsFood;
    bool IsSafe;
    bool IsEnemy;

    CAnt* AntPtr;

    int AntPlayerId, HillPlayer;

    ///======================= FOR A*
    int Dist;
    int TotalCost;
    CSquare* Parent;
    CSquare* AreaCenter;

    //=======================
    bool IsTargeted;
    bool IsReached;
    bool IsReachedByMyBot;

    int Row, Col;

    CSquare* Previous;
    CSquare* FoodSourceFinding;

    ///======================= EXPLORATION
    int ExplorationWeight;
    InternalArray<CSquare*> Prevs;

public:
    CSquare();
    CSquare(int InRow, int InCol);

    void Reset();

    //EDirection GetDirectionTo(CSquare S);

    CSquare* GetNeighbours();

    InternalArray<CSquare*> GetNeighbors();

    bool operator==(const CSquare& Other) const
    {
        return (Row == Other.Row) && (Col == Other.Col);
    }
};

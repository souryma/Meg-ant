#pragma once

#include "Bug.h"

/*
    struct for representing locations in the grid.
*/
struct SLocation
{
    int row, col;

    SLocation()
    {
        row = col = 0;
    };

    SLocation(int r, int c)
    {
        row = r;
        col = c;
    };

    friend Bug& operator<<(Bug& Bug, SLocation& L)
    {
        Bug.file << L.row;
        Bug.file << L.col;

        return Bug;
    };
};

#pragma once

#include "Bug.h"

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    friend Bug& operator<<(Bug& bug, Location& L)
    {
        bug.file << L.row;
        bug.file << L.col;

        return bug;
    };
};

#pragma once

#include <cstdlib>
#include <algorithm>

#include "Square.h"
#include "State.h"
#include "Shared.h"

using namespace std;


class AntManager
{
public:
	void AStar(Square From, Square To, bool FirstStep);

private:


	int Dist(Square S1, Square S2);
};


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
	void AStar(CSquare From, CSquare To, bool FirstStep);

private:


	int Dist(CSquare S1, CSquare S2);
};


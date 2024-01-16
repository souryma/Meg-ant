#pragma once

#include "Bug.h"

/*
	struct for representing locations in the grid.
*/
struct SLocation
{
	int Row, Col;

	friend CBug& operator<<(CBug& Bug, SLocation& L)
	{
		Bug.file << L.Row;
		Bug.file << L.Col;

		return Bug;
	};
};

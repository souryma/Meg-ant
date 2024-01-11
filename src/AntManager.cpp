#include "AntManager.h"


void AntManager::AStar(Square From, Square To, bool StartPoint)
{
	return;
}

int AntManager::Dist(Square S1, Square S2)
{
	const int dCol = abs(S1.Col - S2.Col);
	const int dRow = abs(S1.Row - S2.Col);

	return min(dCol, Shared::Cols - dCol) + min(dRow, Shared::Rows - dRow);
}

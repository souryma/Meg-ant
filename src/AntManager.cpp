#include "AntManager.h"


void AntManager::AStar(CSquare From, CSquare To, bool StartPoint)
{
	return;
}

int AntManager::Dist(CSquare S1, CSquare S2)
{
	const int dCol = abs(S1.Col - S2.Col);
	const int dRow = abs(S1.Row - S2.Col);

	return 1;
}

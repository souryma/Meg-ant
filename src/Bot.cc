#include "Bot.h"

using namespace std;

//constructor
CBot::CBot()
{
	IsTimeout = false;
};

//plays a single game of Ants.
void CBot::PlayGame()
{
	// Init
	cin >> State;
	State.Setup();
	EndTurn();

	CSquare s;

	// Update
	while (cin >> State)
	{
		State.UpdateVisionInformation();

		_InitStrategy();

		_FindFood();

		//_Explore();

		// AssignMissions();

		// Move in a random direction
		for (auto ant : State.MyAnts)
		{
			MakeMoves(*ant->SquarePtr, ant->Explore());
		}

		EndTurn();
	}
};

//makes the CBots moves for the turn
void CBot::MakeMoves(CSquare& From, CSquare& To)
{
	MakeMoves(From, CUtilityFunctions::GetDirectionFromMovement(From, To));
};

//makes the CBots moves for the turn
void CBot::MakeMoves(CSquare& From, int Direction)
{
	State.Bug << "Make move" << endl;

	SLocation loc;
	loc.Row = From.Row;
	loc.Col = From.Col;
	State.MakeMove(loc, Direction);
};

//finishes the turn
void CBot::EndTurn()
{
	if (State.Turn > 0)
		State.Reset();
	State.Turn++;

	cout << "go" << endl;
};

///================ STRATEGY

void CBot::_InitStrategy()
{
	IsTimeout = false;

	_InitNearbyAllies();
	_InitNearbyEnemies();
	_InitExploration();
}

void CBot::_AssignMissions()
{
	for (auto it = Missions.begin(); it != Missions.end();)
	{
		SMission& missionRef = *it;

		if (missionRef.HasToRemove)
		{
			Missions.erase(it);
			continue;
		}
		else
		{
			CAnt* antPtr = missionRef.CurrentSquare.AntPtr;

			if (antPtr == nullptr) // No ant at this square. May be the ant is dead.
			{
				Missions.erase(it);
				continue;
			}

			antPtr->HasMission = true;
			antPtr->MissionPtr = &missionRef;
			++it;
		}
	}
}

void CBot::_FindFood()
{
	InternalMap<CSquare*, bool> isEnemyNearFood;
	InternalList<CSquare*> squaresToExplore;
	InternalList<CSquare*> exploredSquare;

	for (auto food : State.Foods) {
		squaresToExplore.push_back(food);
		food->Dist = 0;
		food->IsReached = true;
		food->FoodSourceFinding = food;
		isEnemyNearFood.insert({ food, false });
		exploredSquare.push_back(food);
	}

	State.Bug << "How many fooood ? " << State.Foods.size() << endl;

	while (!squaresToExplore.empty()) {
		CSquare* currentSquare = squaresToExplore.front();
		squaresToExplore.pop_front();


		// Check if there is any enemy ant close to this current food.
		if (currentSquare->Dist <= FOOD_CLOSE_DIST && currentSquare->AntPlayerId != MY_PLAYER_ID)
		{
			State.Bug << "Enemy close to food." << std::endl;
			isEnemyNearFood.insert({ currentSquare->FoodSourceFinding, true });
		}

		// If you're far away and the enemy is close, do nothing.
		if (currentSquare->Dist > FOOD_CLOSE_DIST && isEnemyNearFood[currentSquare->FoodSourceFinding]) {
			State.Bug << "A" << endl;

			auto it = exploredSquare.begin();
			while (it != exploredSquare.end()) {
				CSquare* s = *it;

				if (s->FoodSourceFinding->ID == currentSquare->FoodSourceFinding->ID) {
					s->IsReached = false;
					it = exploredSquare.erase(it);
				}
				else {
					it++;
				}
			}

			it = squaresToExplore.begin();
			while (it != squaresToExplore.end()) {
				if (!(*it)->IsReached) {
					it = squaresToExplore.erase(it);
				}
				else {
					it++;
				}
			}
		}
		else if (
			currentSquare->AntPlayerId == MY_PLAYER_ID && (currentSquare->AntPtr != nullptr && !currentSquare->AntPtr->HasMoved) &&
			(currentSquare->Previous == nullptr || currentSquare->Previous->AntPlayerId != MY_PLAYER_ID) && !_IsSuicide(*currentSquare->AntPtr, *currentSquare->Previous)
			)
		{

			State.Bug << "B" << endl;

			if (currentSquare->Previous == currentSquare->FoodSourceFinding) {
				currentSquare->AntPtr->HasMoved = true;

				State.Bug << "Has moved ?!" << std::endl;
			}
			else {
				if (!_IsSafe(*currentSquare->Previous)) {

					InternalMap<int, CAnt*> closeAnts = _GetCloseAnts(*currentSquare->FoodSourceFinding);

					bool IsHavingSupport = false;

					for (const auto closeAnt : closeAnts) {
						if (closeAnt.second->IsEnemy || closeAnt.second == currentSquare->AntPtr) {
							continue;
						}
						if (!closeAnt.second->IsEnemy) {
							IsHavingSupport = true;
							break;
						}
					}
					if (IsHavingSupport) {
						MakeMoves(*currentSquare, *currentSquare->Previous);

						State.Bug << "Move !" << std::endl;
					}
				}
				else {
					// square is safe
					MakeMoves(*currentSquare, *currentSquare->Previous);
				}
			}

			auto it = exploredSquare.begin();

			while (it != exploredSquare.end()) {
				CSquare* t = *it;
				if (t->FoodSourceFinding == currentSquare->FoodSourceFinding) {
					t->IsReached = false;
					it = exploredSquare.erase(it);
				}
				else {
					++it;
				}
			}

			it = squaresToExplore.begin();
			while (it != squaresToExplore.end()) {
				if (!(*it)->IsReached) {
					it = squaresToExplore.erase(it);
				}
				else {
					++it;
				}
			}
		}
		else if (currentSquare->Dist < 13) {

			for (auto neighbor : currentSquare->GetNeighbors()) {
				if (neighbor->IsReached)
					continue;

				neighbor->IsReached = true;
				neighbor->Previous = currentSquare;
				neighbor->Dist = currentSquare->Dist + 1;
				neighbor->FoodSourceFinding = currentSquare->FoodSourceFinding;
				exploredSquare.push_back(neighbor);
				squaresToExplore.push_back(neighbor);
			}
		}
	}

	for (auto square : exploredSquare)
		square->IsReached = false;
}

void CBot::_Explore()
{
	for (auto ant : State.MyAnts)
	{
		InternalMap<CSquare*, int> values;
		InternalList<CSquare*> squaresToExplore;
		InternalList<CSquare*> exploredSquares;
		CSquare* antSquare = ant->SquarePtr;

		antSquare->IsReached = true;
		antSquare->Dist = 0;
		exploredSquares.push_back(antSquare);

		for (auto neighbor : antSquare->GetNeighbors()) {
			values.insert({ neighbor, 0 });
			squaresToExplore.push_back(neighbor);
			neighbor->Dist = 1;
			neighbor->IsReached = true;
			neighbor->Prevs.push_back(neighbor);
			exploredSquares.push_back(neighbor);
		}

		while (!squaresToExplore.empty()) {
			CSquare* currentSquare = squaresToExplore.front();
			squaresToExplore.pop_front();

			if (currentSquare->Dist > AREA_DIAMETER) {
				for (auto prevFirst : currentSquare->Prevs) {
					values.insert({ prevFirst, values.at(prevFirst) + currentSquare->ExplorationWeight });
				}
				continue;
			}

			for (auto neighbor : currentSquare->GetNeighbors()) {
				if (neighbor->IsReached) {

					if (neighbor->Dist == currentSquare->Dist + 1) {
						neighbor->Prevs.push_back(*neighbor->Prevs.end());
						neighbor->Prevs.push_back(*currentSquare->Prevs.begin());
						neighbor->Prevs.push_back(*currentSquare->Prevs.end());
					}
					continue;
				}
				neighbor->IsReached = true;
				neighbor->Previous = currentSquare;
				neighbor->Dist = currentSquare->Dist + 1;
				neighbor->Prevs.push_back(*neighbor->Prevs.end());
				neighbor->Prevs.push_back(*currentSquare->Prevs.begin());
				neighbor->Prevs.push_back(*currentSquare->Prevs.end());
				exploredSquares.push_back(neighbor);
				squaresToExplore.push_back(neighbor);
			}
		}

		int bestValue = 0;
		CSquare* bestDest = nullptr;
		for (const auto entry : values) {
			if (entry.second > bestValue) {
				bestValue = entry.second;
				bestDest = entry.first;
			}
		}

		if (bestValue == 0 || bestDest == nullptr) {
			for (auto square : exploredSquares)
				square->IsReached = false;
		}

		for (auto square : exploredSquares) {
			if (square->Dist > AREA_DIAMETER && std::find(square->Prevs.begin(), square->Prevs.end(), bestDest) != square->Prevs.end()) {
				square->ExplorationWeight = 0;
			}
			square->IsReached = false;
			square->Prevs.clear();
		}

		State.Bug << "Owééé" << endl;
		State.Bug << "ANT : " << (!!antSquare) << endl;
		State.Bug << "Owééé" << (!!bestDest) << endl;

		MakeMoves(*antSquare, *bestDest);
	}
}

inline void CBot::_InitNearbyAllies()
{
	for (auto ant1 : State.MyAnts)
	{
		for (auto ant2 : State.MyAnts)
		{
			if (ant1->ID == ant2->ID)
				continue;

			int dRow = CUtilityFunctions::DistRow(*ant1->SquarePtr, *ant2->SquarePtr);

			if (dRow <= MY_ANT_CLOSE_DIST)
			{
				int dCol = CUtilityFunctions::DistCol(*ant1->SquarePtr, *ant2->SquarePtr);

				if (dCol <= MY_ANT_CLOSE_DIST)
				{
					ant1->AlliesCount++;
					ant2->AlliesCount++;
				}
			}
		}
	}
}

inline void CBot::_InitNearbyEnemies()
{
	for (auto myAnt : State.MyAnts)
	{
		for (auto enemyAnt : State.EnemyAnts)
		{
			int dRow = CUtilityFunctions::DistRow(*myAnt->SquarePtr, *enemyAnt->SquarePtr);
			int dCol = CUtilityFunctions::DistCol(*myAnt->SquarePtr, *enemyAnt->SquarePtr);

			if (dRow + dCol <= ENEMY_CLOSEST_DIST)
			{
				if (dRow != 0 && dCol != 0)
				{
					myAnt->ClosestEnemies.push_back(enemyAnt);
					myAnt->EnemyCount++;
					enemyAnt->EnemyCount++;
					continue;
				}
			}

			if (dRow + dCol <= ENEMY_CLOSER_DIST)
			{
				if (dRow != 0 && dCol != 0)
				{
					myAnt->CloserEnemies.push_back(enemyAnt);
					myAnt->EnemyCount++;
					enemyAnt->EnemyCount++;
					continue;
				}
			}


			if (dRow > ENEMY_CLOSE_DIST) continue; // the enemy is far from us
			if (dCol > ENEMY_CLOSE_DIST) continue; // the enemy is far from us

			int dist = dRow * dRow + dCol * dCol;
			if (dist > ENEMY_CLOSE_DIST2) continue; // the enemy is far from us

			myAnt->EnemyCount++;
			myAnt->CloseEnemies.push_back(enemyAnt);
			enemyAnt->EnemyCount++;
		}
	}
}

inline void CBot::_InitFightAreas()
{

}

inline void CBot::_InitExploration()
{
	InternalList<CSquare*> squaresToExplore;
	InternalList<CSquare*> exploratedSquares;

	for (auto ant : State.MyAnts)
		squaresToExplore.push_back(ant->SquarePtr);

	for (auto square : squaresToExplore) {
		square->Dist = 0;
		square->IsReached = true;
		square->AreaCenter = square;
		exploratedSquares.push_back(square);
	}

	while (!squaresToExplore.empty()) {
		auto currentSquare = squaresToExplore.front();
		squaresToExplore.pop_front();

		if (currentSquare->Dist > AREA_DIAMETER) break;

		currentSquare->ExplorationWeight = 0;
		for (auto neighbor : currentSquare->GetNeighbors()) {

			if (neighbor->IsReached)
				continue;

			neighbor->IsReached = true;
			neighbor->Previous = currentSquare;
			neighbor->Dist = currentSquare->Dist + 1;
			neighbor->AreaCenter = currentSquare->AreaCenter;
			exploratedSquares.push_back(neighbor);
			squaresToExplore.push_back(neighbor);
		}
	}
	// Reset values
	for (auto square : exploratedSquares) square->IsReached = false;
}

///================ MISC

InternalMap<int, CAnt*> CBot::_GetCloseAnts(CSquare& Square) {
	InternalMap<int, CAnt*> closeAntDists;

	for (int i = 0; i < State.MyAnts.size();)
	{
		auto ant = State.MyAnts[i];
		int dist = CUtilityFunctions::Dist(*ant->SquarePtr, Square);

		if (dist < MY_ANT_CLOSE_DIST)
		{
			if (closeAntDists.size() > 2)
				i += State.MyAnts.size();

			closeAntDists.insert({ dist, ant });
		}

		i++;
	}

	for (int i = 0; i < State.EnemyAnts.size();)
	{
		auto ant = State.EnemyAnts[i];
		int dist = CUtilityFunctions::Dist(*ant->SquarePtr, Square);

		if (dist < MY_ANT_CLOSE_DIST)
		{
			if (closeAntDists.size() > 2)
				i += State.EnemyAnts.size();

			closeAntDists.insert({ dist, ant });
		}

		i++;
	}

	return closeAntDists;
}

bool CBot::_IsSafe(CSquare& Square)
{
	int row = Square.Row;
	int col = Square.Col;

	return false;
}

bool CBot::_IsSuicide(CAnt& Ant, CSquare& Dest)
{
	for (auto enemyAnt : Ant.CloserEnemies) {
		if (CUtilityFunctions::IsTooCloseTo(*enemyAnt->SquarePtr, Dest))
			return true;

		if (CUtilityFunctions::IsCloseTo(*enemyAnt->SquarePtr, Dest))
			return false;
	}

	return false;
}

/**
* Pathfinding : A*
*/

InternalArray<CSquare*> CBot::_AStar(CSquare& From, CSquare& To)
{
	InternalArray<CSquare*> squaresToExplore;
	InternalArray<CSquare*> exploredSquares;

	squaresToExplore.push_back(&From);

	while (!squaresToExplore.empty())
	{
		int currentIdx = 0;
		CSquare* currentSquare = squaresToExplore[0];

		for (int i = 1; i < squaresToExplore.size(); i++)
		{
			if (squaresToExplore[i]->TotalCost < currentSquare->TotalCost)
			{
				currentSquare = squaresToExplore[i];
				currentIdx = i;
			}
		}


		squaresToExplore.erase(squaresToExplore.begin() + currentIdx);
		exploredSquares.push_back(currentSquare);


		if (currentSquare->Row == To.Row && currentSquare->Col == To.Col) {
			InternalArray<CSquare*> path;

			while (currentSquare != nullptr) {
				path.push_back(currentSquare);
				currentSquare = currentSquare->Parent;
			}
			return path;
		}

		// Generate neighbors
		InternalArray<CSquare*> neighbors;

		int dRow = -1, dCol = 0;
		int row = 0, col = 0;

		// Horizontal
		row = currentSquare->Row + dRow;
		col = currentSquare->Col + dCol;

		if (_IsSafe(*SGlobal::Grid[row][col])) neighbors.push_back(new CSquare(row, col));

		dRow = 1;
		row = currentSquare->Row + dRow;

		if (_IsSafe(*SGlobal::Grid[row][col]))
			neighbors.push_back(new CSquare(row, col));

		// Vertical
		dRow = 0, dCol = -1;

		row = currentSquare->Row + dRow;
		col = currentSquare->Col + dCol;

		if (_IsSafe(*SGlobal::Grid[row][col]))
			neighbors.push_back(new CSquare(row, col));

		dCol = 1;
		col = currentSquare->Col + dCol;

		if (_IsSafe(*SGlobal::Grid[row][col]))
			neighbors.push_back(new CSquare(row, col));


		for (auto neighbor : neighbors) {

			if (_IsInSquareArray(exploredSquares, *neighbor)) {
				delete neighbor;
				continue;
			}

			// Neighbors costs
			int neighborG = currentSquare->Dist + 1;
			int neighborH = _AStarHeuristic(*neighbor, To);
			int neighborF = neighborG + neighborH;


			bool isOpen = _IsInSquareArray(squaresToExplore, *neighbor);
			if (!isOpen || neighborF < neighbor->TotalCost) {
				// update costs
				neighbor->Dist = neighborG;
				neighborH = neighborH;
				neighbor->TotalCost = neighborG + neighborH;
				neighbor->Parent = currentSquare;

				// Add neighbor
				if (!isOpen) {
					squaresToExplore.push_back(neighbor);
				}
			}
			else {
				delete neighbor;
			}
		}

	}

	return InternalArray<CSquare*>();
}

bool CBot::_IsInSquareArray(const InternalArray<CSquare*> Arr, const CSquare& Square)
{
	for (const auto square : Arr) {
		if (square->Row == Square.Row && square->Col == Square.Col) {
			return true;
		}
	}
	return false;
}

inline int CBot::_AStarHeuristic(const CSquare& S1, const CSquare& S2)
{
	return std::abs(S1.Row - S2.Col) + std::abs(S1.Col - S2.Col);
}


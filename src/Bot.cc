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
	endTurn();

	CSquare s;

	// Update
	while (cin >> State)
	{
		State.UpdateVisionInformation();

		_InitStrategy();

		_FindFood();

		// CreateMissions();
		// AssignMissions();
		// DefendHill();
		// Attack();
		// Explore();
		// Dance();
		endTurn();
	}
};

//makes the CBots moves for the turn
void CBot::MakeMoves(CSquare& From, CSquare& To)
{
	//State.Bug << "turn " << State.Turn << ":" << endl;
	//State.Bug << State << endl;

	//// TODO find a way to split exploring ants and defending ants

	//SLocation closestFood;
	////Make every exploring ants go the closest food
	//for (int ant = 0; ant < (int)State.MyAnts.size(); ant++)
	//{
	//	for (int d = 0; d < TDIRECTIONS; d++)
	//	{
	//		SLocation loc = State.GetLocation(State.MyAnts[ant]->Location, d);

	//		if (!SGlobal::Grid[loc.Row][loc.Col]->IsWater)
	//		{
	//			//closestFood = getClosestFood(State.myAnts[ant]);
	//			//// check if food is not already targeted by an ant
	//			//if (!isFoodAlreadyTargeted(closestFood)) 
	//			//{
	//			//	State.targetedFoods.push_back(closestFood);
	//			//}
	//			State.Bug << "Closest food : " << closestFood << endl;

	//			// TODO get shorest path between ants and closest food (A*?)

	//			State.makeMove(State.MyAnts[ant]->Location, d);
	//			break;
	//		}
	//	}
	//}


	State.Bug << "Make move" << endl;

	SLocation loc;
	loc.Row = From.Row;
	loc.Col = From.Col;
	State.makeMove(loc, CUtilityFunctions::GetDirectionFromMovement(From, To));
};

//finishes the turn
void CBot::endTurn()
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

void CBot::_CreateMissions()
{
	//for (CAnt ant : State.MyAnts)
	//{
	//	if (ant.HasMission) continue; // Verifier si la mission est bonne

	//	CSquare targetSquare;
	//	
	//	
	//	
	//}

	// Analyser les foods
	// Analyser les enemies
	// Analyser les alliés
	// Analyser les 

}

void CBot::_DefendHill()
{
}

void CBot::_AttackEnemyHills()
{
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
					
					InternalMap<int, CAnt*> closeAnts = GetCloseAnts(*currentSquare->FoodSourceFinding);

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
					// tile is safe
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

	for (auto tile : exploredSquare)
		tile->IsReached = false;
}

void CBot::_Explore()
{
}

void CBot::_Attack()
{
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
	//InternalList<CSquare*> squaresToExplore;
	//InternalList<CSquare*> exploredSquare;

	//for (auto AntPlayerId : State.EnemyAnts) {
	//	squaresToExplore.push_back(AntPlayerId->SquarePtr);
	//}
	//for (auto AntPlayerId : State.MyAnts) {
	//	squaresToExplore.push_back(AntPlayerId->SquarePtr);
	//	AntPlayerId->IsInMyArea = true;
	//}

	//for (auto currentSquare : squaresToExplore) {
	//	currentSquare->Dist = 0;
	//	currentSquare->IsReached = true;
	//	exploredSquare.push_back(currentSquare);
	//	currentSquare->AreaCenter = currentSquare;
	//	SFightArea area;
	//	if (currentSquare->IsHill && currentSquare->HillPlayer == MY_PLAYER_ID) {
	//		area.IsAreaContainsMyHill = true;
	//		area.MyHill = currentSquare;
	//	}
	//	area.Player = currentSquare->AntPlayerId;
	//	area.Ants.push_back(currentSquare->AntPtr);
	//	area.Squares.push_back(currentSquare);
	//	areaMap[currentSquare] = area;
	//}
	//while (!squaresToExplore.empty()) {
	//	CSquare* currentSquare = squaresToExplore.front();
	//	squaresToExplore.pop_front();

	//	if (currentSquare->Dist >= AREA_DIAMETER) break;
	//	
	//	for (auto n : currentSquare->GetNeighbors()) {
	//		if (n.isReached) {
	//			if (n.startTile != currentSquare.startTile && n.startTile->type == currentSquare.startTile->type &&
	//				areaMap[n.startTile] != areaMap[currentSquare.startTile]) {
	//				Area& nArea = areaMap[n.startTile];
	//				Area& tileArea = areaMap[currentSquare.startTile];
	//				tileArea.ants.splice(tileArea.ants.end(), nArea.ants);
	//				tileArea.tiles.splice(tileArea.tiles.end(), nArea.tiles);
	//				for (auto& AntPlayerId : nArea.ants) {
	//					areaMap[AntPlayerId.currentSquare] = tileArea;
	//				}
	//				if (nArea.containsHill) {
	//					tileArea.containsHill = true;
	//					tileArea.hill = nArea.hill;
	//				}
	//			}
	//		}
	//		else {
	//			n.isReached = true;
	//			n.dist = currentSquare.dist + 1;
	//			n.startTile = currentSquare.startTile;
	//			areaMap[n.startTile].tiles.push_back(n);
	//			if (n.startTile->type == MY_ANT) n.isInMyArea = true;
	//			if (n.isHill && n.HillPlayer == MY_ANT) {
	//				areaMap[n.startTile].containsHill = true;
	//				areaMap[n.startTile].hill = &n;
	//			}
	//			exploredSquare.push_back(n);
	//			squaresToExplore.push_back(n);
	//		}
	//	}
	//}
	//for (auto& tile : exploredSquare) {
	//	tile.isReached = false;
	//}
}

///================ MISC

InternalMap<int, CAnt*> CBot::GetCloseAnts(CSquare& Square) {
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

//InternalArray<CSquare&> CBot::_BFS_DistanceBased(CSquare& Start, int MaxDistance)
//{
//	InternalQueue<CSquare&> squaresToExplore;
//	InternalQueue<CSquare&> exploredSquare;
//
//	Start.Dist = 0;
//	Start.IsReached = true;
//	squaresToExplore.push(Start);
//
//	while (!squaresToExplore.empty())
//	{
//		CSquare& currentSquare = squaresToExplore.front();
//		squaresToExplore.pop();
//
//		if (currentSquare.Dist >= MaxDistance)
//			break;
//
//		for (auto neighborPtr : currentSquare.GetNeighbors())
//		{
//			if (neighborPtr->IsReached)
//			{
//
//			}
//			else
//			{
//				neighborPtr->IsReached = true;
//				neighborPtr->Dist = currentSquare.Dist + 1;
//
//
//			}
//		}
//	}
//
//	return InternalArray<CSquare&>();
//}

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


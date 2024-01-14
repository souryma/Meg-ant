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

	// Update
	while (cin >> State)
	{
		State.UpdateVisionInformation();
		// CreateMissions();
		// AssignMissions();
		// DefendHill();
		// Attack();
		// Explore();
		// Dance();

		makeMoves();
		endTurn();
	}
};

//makes the CBots moves for the turn
void CBot::makeMoves()
{
	//State.bug << "turn " << State.Turn << ":" << endl;
	//State.bug << State << endl;

	//// DEBUG : for now, every ant is an exploring ant
	//State.exploringAnts = State.myAnts;

	//// TODO find a way to split exploring ants and defending ants

	//SLocation closestFood;
	////Make every exploring ants go the closest food
	//for (int ant = 0; ant < (int)State.exploringAnts.size(); ant++)
	//{
	//	for (int d = 0; d < TDIRECTIONS; d++)
	//	{
	//		SLocation loc = State.getLocation(State.MyAnts[ant], d);

	//		if (!State.Grid[loc.row][loc.col].isWater)
	//		{
	//			//closestFood = getClosestFood(State.myAnts[ant]);
	//			//// check if food is not already targeted by an ant
	//			//if (!isFoodAlreadyTargeted(closestFood)) 
	//			//{
	//			//	State.targetedFoods.push_back(closestFood);
	//			//}
	//			State.bug << "Closest food : " << closestFood << endl;

	//			// TODO get shorest path between ants and closest food (A*?)

	//			State.makeMove(State.MyAnts[ant].Location, d);
	//			break;
	//		}
	//	}
	//}

	//State.bug << "time taken: " << State.timer.getTime() << "ms" << endl << endl;
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
			antPtr->Mission = &missionRef;
			++it;
		}
	}
}

void CBot::_CreateMissions()
{
	for (CAnt ant : State.MyAnts)
	{
		if (ant.HasMission) continue; // Verifier si la mission est bonne

		CSquare targetSquare;
		
		
		
	}

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
}

void CBot::_Explore()
{
}

void CBot::_Attack()
{
}

///================ MISC

void CBot::_AStar(CSquare& From, CSquare& To, bool FirstTree)
{
	bool found = false;
	const int maxDist = 400;

	InternalList<CSquare> squaresToExplore;
	InternalList<CSquare> exploredSquares;

	squaresToExplore.push_back(From);
	int f = 0;
	int fromDist = 0;
	bool bReached = true;
	exploredSquares.push_back(From);

	for (auto it = squaresToExplore.begin(); it != squaresToExplore.end();)
	{
		CSquare& currentSquare = *it;
		int currentSquareDist = 0;

		CSquare* neighbours = currentSquare.GetNeighbours();

		for (int i = 0; i < 4 ; i++)
		{
			auto neighbour = *(neighbours + i);

			if (
				neighbour.IsReached ||
				(currentSquare == From && (FirstTree && !_IsSafe(neighbour))) ||
				neighbour.IsHill && neighbour.hillPlayer == MY_PLAYER_ID
			)
				continue;

			int neighbourDist = currentSquareDist + 1;

		}

		delete[] neighbours;
	}
}

bool CBot::_IsSafe(CSquare& Dest)
{
	return false;
}


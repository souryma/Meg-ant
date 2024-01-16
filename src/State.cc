#include "State.h"

using namespace std;

//constructor
CState::CState()
{
	_GameOver = 0;
	Turn = 0;
	Bug.open("./debug.txt");
};

//deconstructor
CState::~CState()
{
	Bug.close();
};

//sets the state up
void CState::Setup()
{
	SGlobal::Grid = InternalArray2D<CSquare*>(Rows, InternalArray<CSquare*>(Cols, new CSquare()));
};

//resets all non-water squares to land and clears the bots ant vector
void CState::Reset()
{
	MyAnts.clear();
	EnemyAnts.clear();
	MyHills.clear();
	EnemyHills.clear();
	Foods.clear();
	for (int row = 0; row < Rows; row++)
		for (int col = 0; col < Cols; col++)
			if (!SGlobal::Grid[row][col]->IsWater)
				SGlobal::Grid[row][col]->Reset();
};

//outputs move information to the engine
void CState::MakeMove(SLocation& loc, int direction)
{
	Bug << "o " << loc.Row << " " << loc.Col << " " << CDIRECTIONS[direction] << endl;
	cout << "o " << loc.Row << " " << loc.Col << " " << CDIRECTIONS[direction] << endl;

	SetLocation(loc, direction);
	SGlobal::Grid[loc.Row][loc.Col]->AntPlayerId = SGlobal::Grid[loc.Row][loc.Col]->AntPlayerId;
	SGlobal::Grid[loc.Row][loc.Col]->AntPlayerId = -1;
};

//returns the euclidean distance between two locations with the edges wrapped
double CState::Distance(const SLocation& loc1, const SLocation& loc2)
{
	int d1 = abs(loc1.Row - loc2.Row),
		d2 = abs(loc1.Col - loc2.Col),
		dr = min(d1, Rows - d1),
		dc = min(d2, Cols - d2);
	return sqrt(dr * dr + dc * dc);
};

//returns the new location from moving in a given direction with the edges wrapped
void CState::SetLocation(SLocation& IOLocation, int Direction)
{
	IOLocation.Row = (IOLocation.Row + DIRECTIONS[Direction][0] + Rows) % Rows;
	IOLocation.Col = (IOLocation.Col + DIRECTIONS[Direction][1] + Cols) % Cols;
};

/*
	This function will update update the lastSeen value for any squares currently
	visible by one of your live ants.

	BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
	THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
	A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
	IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void CState::UpdateVisionInformation()
{
	std::queue<SLocation> locQueue;
	SLocation sLoc, cLoc;

	for (int a = 0; a < (int)MyAnts.size(); a++)
	{
		sLoc = MyAnts[a]->Location;
		locQueue.push(sLoc);

		std::vector<std::vector<bool> > visited(Rows, std::vector<bool>(Cols, 0));
		SGlobal::Grid[sLoc.Row][sLoc.Col]->IsVisible = 1;
		visited[sLoc.Row][sLoc.Col] = 1;

		while (!locQueue.empty())
		{
			cLoc = locQueue.front();
			locQueue.pop();

			for (int d = 0; d < TDIRECTIONS; d++)
			{
				SetLocation(cLoc, d);

				if (!visited[cLoc.Row][cLoc.Col] && Distance(sLoc, cLoc) <= ViewRadius)
				{
					SGlobal::Grid[cLoc.Row][cLoc.Col]->IsVisible = 1;
					locQueue.push(cLoc);

					// TODO : here we should fill the state.food / state.enemy / ... 
				}
				visited[cLoc.Row][cLoc.Col] = 1;
			}
		}
	}
};

/*
	This is the output function for a state. It will add a char map
	representation of the state to the output stream passed to it.

	For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream& os, const CState& state)
{
	for (int row = 0; row < state.Rows; row++)
	{
		for (int col = 0; col < state.Cols; col++)
		{
			if (SGlobal::Grid[row][col]->IsWater)
				os << '%';
			else if (SGlobal::Grid[row][col]->IsFood)
				os << '*';
			else if (SGlobal::Grid[row][col]->IsHill)
				os << (char)('A' + SGlobal::Grid[row][col]->HillPlayer);
			else if (SGlobal::Grid[row][col]->AntPlayerId >= 0)
				os << (char)('a' + SGlobal::Grid[row][col]->AntPlayerId);
			else if (SGlobal::Grid[row][col]->IsVisible)
				os << '.';
			else
				os << '?';
		}
		os << endl;
	}

	return os;
};

//input function
istream& operator>>(istream& is, CState& state)
{
	int row, col, player;
	string inputType, junk;

	//finds out which turn it is
	while (is >> inputType)
	{
		if (inputType == "end")
		{
			state._GameOver = 1;
			break;
		}
		else if (inputType == "turn")
		{
			is >> state.Turn;
			break;
		}
		else //unknown line
			getline(is, junk);
	}

	if (state.Turn == 0)
	{
		//reads game parameters
		while (is >> inputType)
		{
			if (inputType == "loadtime")
				is >> state.LoadTime;
			else if (inputType == "turntime")
				is >> state.TurnTime;
			else if (inputType == "rows")
			{
				is >> state.Rows;
				SGlobal::Rows = state.Rows;
				state.Bug << "ROWS : " << SGlobal::Rows << endl;
			}
			else if (inputType == "cols")
			{
				is >> state.Cols;
				SGlobal::Cols = state.Cols;
				state.Bug << "COLS : " << SGlobal::Cols << endl;
			}
			else if (inputType == "turns")
				is >> state.TotalTurns;
			else if (inputType == "player_seed")
				is >> state._Seed;
			else if (inputType == "viewradius2")
			{
				is >> state.ViewRadius;
				state.ViewRadius = sqrt(state.ViewRadius);
			}
			else if (inputType == "attackradius2")
			{
				is >> state.AttackRadius;
				state.AttackRadius = sqrt(state.AttackRadius);
			}
			else if (inputType == "spawnradius2")
			{
				is >> state.SpawnRadius;
				state.SpawnRadius = sqrt(state.SpawnRadius);
			}
			else if (inputType == "ready") //end of parameter input
			{
				state.Timer.Start();
				break;
			}
			else    //unknown line
				getline(is, junk);
		}
	}
	else
	{
		//reads information about the current turn
		while (is >> inputType)
		{
			if (inputType == "w") //water square
			{
				is >> row >> col;
				SGlobal::Grid[row][col]->IsWater = true;
				SGlobal::Grid[row][col]->IsSafe = false;
			}
			else if (inputType == "f") //food square
			{
				is >> row >> col;
				SGlobal::Grid[row][col]->IsFood = 1;
				SGlobal::Grid[row][col]->IsSafe = true;

				CSquare* square = new CSquare(row, col);
				square->IsFood = true;

				state.Foods.push_back(square);
			}
			else if (inputType == "a") //live ant square
			{
				is >> row >> col >> player;

				CSquare* square = new CSquare(row, col);
				CAnt* ant = new CAnt(row, col);
				square->AntPtr = ant;
				square->AntPlayerId = player;
				ant->SquarePtr = square;

				SGlobal::Grid[row][col]->AntPlayerId = player;
				SGlobal::Grid[row][col]->AntPtr = ant;
				SGlobal::Grid[row][col] = square;

				if (player == MY_PLAYER_ID)
				{
					square->IsSafe = true;
					state.MyAnts.push_back(ant);
				}
				else
				{
					square->IsSafe = false;
					state.EnemyAnts.push_back(ant);
				}
			}
			else if (inputType == "d") //dead ant square
			{
				// Is not really useful I think

				//is >> row >> col >> player;
				//state.SGlobal::Grid[row][col].deadAnts.push_back(player);
			}
			else if (inputType == "h")
			{
				is >> row >> col >> player;

				CSquare* square = new CSquare(row, col);
				SGlobal::Grid[row][col] = square;

				square->IsHill = true;
				square->HillPlayer = player;

				if (player == MY_PLAYER_ID)
				{
					square->HillPlayer = MY_PLAYER_ID;

					state.MyHills.push_back(square);
				}
				else
				{
					square->HillPlayer = 2;

					state.EnemyHills.push_back(square);
				}

			}
			else if (inputType == "players") //player information
				is >> state.NumberOfPlayers;
			else if (inputType == "scores") //score information
			{
				state.Scores = vector<double>(state.NumberOfPlayers, 0.0);
				for (int p = 0; p < state.NumberOfPlayers; p++)
					is >> state.Scores[p];
			}
			else if (inputType == "go") //end of turn input
			{
				if (state._GameOver)
					is.setstate(std::ios::failbit);
				else
					state.Timer.Start();
				break;
			}
			else //unknown line
				getline(is, junk);
		}
	}

	return is;
};

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
    Grid = vector<vector<CSquare> >(Rows, vector<CSquare>(Cols, CSquare()));
};

//resets all non-water squares to land and clears the bots ant vector
void CState::Reset()
{
    MyAnts.clear();
    EnemyAnts.clear();
    MyHills.clear();
    EnemyHills.clear();
    Foods.clear();
    for(int row=0; row<Rows; row++)
        for(int col=0; col<Cols; col++)
            if(!Grid[row][col].IsWater)
                Grid[row][col].Reset();
};

//outputs move information to the engine
void CState::makeMove(const SLocation &loc, int direction)
{
    cout << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << endl;

    SLocation nLoc = GetLocation(loc, direction);
    Grid[nLoc.row][nLoc.col].ant = Grid[loc.row][loc.col].ant;
    Grid[loc.row][loc.col].ant = -1;
};

//returns the euclidean distance between two locations with the edges wrapped
double CState::Distance(const SLocation &loc1, const SLocation &loc2)
{
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, Rows-d1),
        dc = min(d2, Cols-d2);
    return sqrt(dr*dr + dc*dc);
};

//returns the new location from moving in a given direction with the edges wrapped
SLocation CState::GetLocation(const SLocation &loc, int direction)
{
    return SLocation( (loc.row + DIRECTIONS[direction][0] + Rows) % Rows,
                     (loc.col + DIRECTIONS[direction][1] + Cols) % Cols );
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
    SLocation sLoc, cLoc, nLoc;

    for(int a=0; a<(int) MyAnts.size(); a++)
    {
        sLoc = MyAnts[a].Location;
        locQueue.push(sLoc);

        std::vector<std::vector<bool> > visited(Rows, std::vector<bool>(Cols, 0));
        Grid[sLoc.row][sLoc.col].IsVisible = 1;
        visited[sLoc.row][sLoc.col] = 1;

        while(!locQueue.empty())
        {
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++)
            {
                nLoc = GetLocation(cLoc, d);

                if(!visited[nLoc.row][nLoc.col] && Distance(sLoc, nLoc) <= ViewRadius)
                {
                    Grid[nLoc.row][nLoc.col].IsVisible = 1;
                    locQueue.push(nLoc);

                    // TODO : here we should fill the state.food / state.enemy / ... 
                }
                visited[nLoc.row][nLoc.col] = 1;
            }
        }
    }
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream &os, const CState &state)
{
    for(int row=0; row<state.Rows; row++)
    {
        for(int col=0; col<state.Cols; col++)
        {
            if(state.Grid[row][col].IsWater)
                os << '%';
            else if(state.Grid[row][col].IsFood)
                os << '*';
            else if(state.Grid[row][col].IsHill)
                os << (char)('A' + state.Grid[row][col].hillPlayer);
            else if(state.Grid[row][col].ant >= 0)
                os << (char)('a' + state.Grid[row][col].ant);
            else if(state.Grid[row][col].IsVisible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
};

//input function
istream& operator>>(istream &is, CState &state)
{
    int row, col, player;
    string inputType, junk;

    //finds out which turn it is
    while(is >> inputType)
    {
        if(inputType == "end")
        {
            state._GameOver = 1;
            break;
        }
        else if(inputType == "turn")
        {
            is >> state.Turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }

    if(state.Turn == 0)
    {
        //reads game parameters
        while(is >> inputType)
        {
            if(inputType == "loadtime")
                is >> state.LoadTime;
            else if(inputType == "turntime")
                is >> state.TurnTime;
            else if(inputType == "rows")
                is >> state.Rows;
            else if(inputType == "cols")
                is >> state.Cols;
            else if(inputType == "turns")
                is >> state.TotalTurns;
            else if(inputType == "player_seed")
                is >> state._Seed;
            else if(inputType == "viewradius2")
            {
                is >> state.ViewRadius;
                state.ViewRadius = sqrt(state.ViewRadius);
            }
            else if(inputType == "attackradius2")
            {
                is >> state.AttackRadius;
                state.AttackRadius = sqrt(state.AttackRadius);
            }
            else if(inputType == "spawnradius2")
            {
                is >> state.SpawnRadius;
                state.SpawnRadius = sqrt(state.SpawnRadius);
            }
            else if(inputType == "ready") //end of parameter input
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
        while(is >> inputType)
        {
            if(inputType == "w") //water square
            {
                is >> row >> col;
                state.Grid[row][col].IsWater = 1;
            }
            else if(inputType == "f") //food square
            {
                is >> row >> col;
                state.Grid[row][col].IsFood = 1;
                state.Foods.push_back(CSquare(row, col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> row >> col >> player;
                state.Grid[row][col].ant = player;
                if(player == 0)
                    state.MyAnts.push_back(CAnt(row, col));
                else
                    state.EnemyAnts.push_back(CAnt(row, col));
            }
            else if(inputType == "d") //dead ant square
            {
                // Is not really useful I think

                //is >> row >> col >> player;
                //state.grid[row][col].deadAnts.push_back(player);
            }
            else if(inputType == "h")
            {
                is >> row >> col >> player;
                state.Grid[row][col].IsHill = 1;
                state.Grid[row][col].hillPlayer = player;

                if(player == 0)
                    state.MyHills.push_back(CSquare(row, col));
                else
                    state.EnemyHills.push_back(CSquare(row, col));

            }
            else if(inputType == "players") //player information
                is >> state.NumberOfPlayers;
            else if(inputType == "scores") //score information
            {
                state.Scores = vector<double>(state.NumberOfPlayers, 0.0);
                for(int p=0; p<state.NumberOfPlayers; p++)
                    is >> state.Scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(state._GameOver)
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

#include "Bot.h"

using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
	//reads the game parameters and sets up
	cin >> state;
	state.setup();
	endTurn();

	//continues making moves while the game is not over
	while (cin >> state)
	{
		state.updateVisionInformation();
		makeMoves();
		endTurn();
	}
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
	state.bug << "turn " << state.turn << ":" << endl;
	state.bug << state << endl;

	// DEBUG : for now, every ant is an exploring ant
	state.exploringAnts = state.myAnts;

	// TODO find a way to split exploring ants and defending ants

	//Make every exploring ants go the closest food
	for (int ant = 0; ant < (int)state.exploringAnts.size(); ant++)
	{
		for (int d = 0; d < TDIRECTIONS; d++)
		{
			Location loc = state.getLocation(state.myAnts[ant], d);

			if (!state.grid[loc.row][loc.col].isWater)
			{
				state.bug << "Closest food : " << getClosestFood(state.myAnts[ant]) << endl;

				// TODO get shorest path between ants and closest food

				state.makeMove(state.myAnts[ant], d);
				break;
			}
		}
	}

	state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

Location Bot::getClosestFood(Location ant) {
	double smallestDistance = 99999.0, previousSmallestDistance = 99999.0;
	Location closestFood = Location(0, 0); // Returns this position of no food is found

	// iterate on each food in the current state
	for (int i = 0; i < state.food.size(); i++) {
		smallestDistance = min(smallestDistance, state.distance(ant, state.food[i]));
		if (smallestDistance != previousSmallestDistance) {
			previousSmallestDistance = smallestDistance;
			closestFood = state.food[i];
		}
	}

	return closestFood;
}

//finishes the turn
void Bot::endTurn()
{
	if (state.turn > 0)
		state.reset();
	state.turn++;

	cout << "go" << endl;
};

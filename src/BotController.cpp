// BotController.h
class BotController {
public:
	BotController(State& state);

	void makeTurn();

private:
	State& state;
	ExplorationManager explorationManager;
	CombatManager combatManager;
};

// ExplorationManager.h
class ExplorationManager {
public:
	void assignTargets();

private:
	std::unordered_map<Ant*, Path> pathsCache;
	State& state;
};

// CombatManager.h
class CombatManager {
public:
	void defendHills();

private:
	std::list<Ant*> defenders;
};

// BotController.cpp
BotController::BotController(State& state)
	: state(state)
	, explorationManager(state)
	, combatManager(state)
{}

void BotController::makeTurn() {
	explorationManager.assignTargets();
	combatManager.defendHills();
}

// ExplorationManager.cpp
void ExplorationManager::assignTargets() {

	// Get closest food path for each explorer ant
	for (Ant* ant : explorers) {
		Location target = getClosestFood(ant->location);

		// Check cache first
		if (pathsCache.count(ant)) {
			ant->path = pathsCache[ant];
		}
		else {
			ant->path = AStarSearch(state, ant->location, target);
			pathsCache[ant] = ant->path;
		}
	}
}
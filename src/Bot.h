#pragma once

#include "State.h"

/*
	This struct represents your bot in the game of Ants
*/
class CBot
{
private:
	InternalList<SMission> Missions;
	InternalArray<SFightArea*> _FightAreas;

public:
	CState State;
	bool IsTimeout;

public:
	CBot();

	void PlayGame();    //plays a single game of Ants

	void MakeMoves(CSquare& From, CSquare& To);   //makes moves for a single turn
	void MakeMoves(CSquare& From, int Direction);   //makes moves for a single turn

	void EndTurn();     //indicates to the engine that it has made its moves

private:
	///================ STRATEGY
	void _InitStrategy();

	void _AssignMissions();

	/*
	* On commence par la food ou par nos fourmis ?
	*/
	void _FindFood();

	/*
	* Dans le cas où on ne détecte pas de bouf au début de la partie.
	* On envoie la/les fourmi(s) en balade.
	* C'est ici qu'on définira le comportement de "longer vers un mur" juste pour rien
	*/
	void _Explore();

	///================ MISC
	inline void _InitNearbyAllies();
	inline void _InitNearbyEnemies();
	inline void _InitFightAreas();
	inline void _InitExploration();

	InternalMap<int, CAnt*> GetCloseAnts(CSquare& Square);

	// No enemy / My ant is there
	// Check the area around the square
	bool _IsSafe(CSquare& Square);

	bool _IsSuicide(CAnt& Ant, CSquare& Dest);

	/**
	* Pathfinding : A*
	*/

	InternalArray<CSquare*> _AStar(CSquare& From, CSquare& To);

	bool _IsInSquareArray(const InternalArray<CSquare*> Arr, const CSquare& Square);

	// Manhattan distance
	inline int _AStarHeuristic(const CSquare& S1, const CSquare& S2);
};
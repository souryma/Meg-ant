#pragma once

#include "State.h"

#define MY_PLAYER_ID 0


/*
    This struct represents your bot in the game of Ants
*/
class CBot
{
public:
    CState State;
    InternalList<SMission> Missions;
    bool IsTimeout;

public:
    CBot();

    void PlayGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves

private:
    ///================ STRATEGY
    void _AssignMissions();
    void _CreateMissions();

    void _DefendHill();

    void _AttackEnemyHills();

    /*
    * On commence par la food ou par nos fourmis ?
    */
    void _FindFood();

    /*
    * Dans le cas o� on ne d�tecte pas de bouf au d�but de la partie.
    * On envoie la/les fourmi(s) en balade.
    * C'est ici qu'on d�finira le comportement de "longer vers un mur" juste pour rien
    */
    void _Explore();

    /*
    * En fonction du nombre de fourmis qu'on a.
    * Si on a 3 fourmi (par exemple) �a sert � rien d'attaquer.
    * Sinon on peut commencer l'ex�cution de la fonction.
    * Voir pour les conditions sur combien de fourmi on envoie pour une attaque donn�e (hill / enemies)
    */
    void _Attack();

    ///================ MISC
    void _AStar(CSquare& From, CSquare& To, bool FirstTree);

    // No enemy / My ant is there
    bool _IsSafe(CSquare& Dest);
};
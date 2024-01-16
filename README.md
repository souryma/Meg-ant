# Style Guide

Ce document décrit les conventions de nommage et de style de code à suivre pour ce projet.

## Nommage

### Types

- Les structures commencent par `S` (ex: `SMission`)
- Les classes commencent par `C` (ex: `CSquare`)
- Les énumérations commencent par `E` (ex: `EDirection`)

### Variables

- Les constantes sont en MAJUSCULES (ex: `MAX_PLAYERS`)
- Les membres de classes commencent par une majuscule (ex: `Name`)
- Les membres privés sont préfixés d'un underscore suivie d'une majuscule (ex: `_Score`)
- Les variables locales commencent par une minuscule (ex: `playerIndex`)

### Espaces de noms

- Les fichiers d'en-tête ont le préfixe `h` (ex: `player.h`)
- Les fichiers sources ont le préfixe `cpp` (ex: `player.cpp`)
- Les espaces de noms utilisent les majuscules (ex: `GAME`)

## Style

- Indentation avec des tabulations de taille 4
- Accolades ouvrantes et fermantes sur la même colonne
- Espaces autour des opérateurs
- Les commentaires sont préfixés de `//`
- Les commentaires de séparation sont préfixés de `///` pour signifier le début d'une section de code

## Autres conventions

- Les types internes redéfinis commencent par `Internal` (ex: `InternalArray`, `InternalList`)

# Problèmes rencontrés

- Compréhension du code de base ainsi que les mécaniques du jeu
- Peu d'affinité avec le C++

Malgré les difficultés rencontrés, nous avons essayé au mieux d'implémenter les principaux algorithmes ainsi que la stratégie que nous avons imaginé. Mais à cause de la structure du code on a pas réussi à tout unifier.
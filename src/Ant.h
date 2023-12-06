#pragma once
#include "Location.h"

class Ant
{
private:
	Location _Location;
public:
	bool IsInDanger;
	bool IsReallyInDanger;
	int nemyCount;

public:
	void Attack();
	void Defend();
	void Explore();
	void Danse();
};


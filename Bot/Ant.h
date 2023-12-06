#pragma once


class Ant
{
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


#pragma once

struct Shared
{
public:
	static int Rows, Cols;
};

int Shared::Rows = 0;
int Shared::Cols = 0;
#pragma once
class Grid
{
public:
	bool newGrid;
	int gridX;
	int gridY;
	int startIndex;
	int endIndex;

	bool inTheGrid(int gridX, int gridY);
};


#include "stdafx.h"
#include "Grid.h"

bool Grid::inTheGrid(int gridX, int gridY)
{
	return gridX == this->gridX && gridY == this->gridY;
}

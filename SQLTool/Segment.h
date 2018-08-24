#pragma once
#include "Point.h"
struct Segment
{
	Point start;
	Point end;
	int startT;//the Trackid the startPoint belong to
	int startIdx;//the idx of startPoint in its track
	int endT;
	int endIdx;
}; 


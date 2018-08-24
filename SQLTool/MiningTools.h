#pragma once
#include "Segment.h"
#include "Point.h"
#include "Vector2D.h"
class MiningTools
{
public:
	MiningTools();
	~MiningTools();
	static double distanceBetweenLines(Segment sg1,Segment sg2,double* weights);
	static double verticalDistBetweenLines(Segment sg1, Segment sg2);
	static double horizontalDistBetweenLines(Segment sg1, Segment sg2);
	static double angleDistBetweenLines(Segment sg1, Segment sg2);
	static Point projectionOfPoint(Segment sg, Point p);
	static double distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude);
	static void segmentRotation(Segment &seg, double angle);
};


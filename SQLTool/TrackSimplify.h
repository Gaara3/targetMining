#pragma once

#include "Track.h"
#include "TrackPoint.h"
#include "SampledTrack.h"
#include <vector>

using std::vector;
class TrackSimplify
{
public:
	TrackSimplify();
	~TrackSimplify();
	static SampledTrack simplify(Track track, int level,double radius);
	static double getTolerance(int level,double radius);
	static SampledTrack similifyRadialDistance(Track, double);
};


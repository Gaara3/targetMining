#pragma once
#include "Track.h"
class SampledTrack :
	public Track
{
	int sampleLevel;
public:
	//vector<int> originalOrderNumber;
	void setSampleLevel(int level);
	SampledTrack();
	SampledTrack(Track t,int level);
	~SampledTrack();
	void insertSampledDetail();
};


#pragma once
#include "Processor.h"
#include "Track.h"
#include "EpsiSolver.h"
#include "DBSCAN.h"
#include "Cluster.h"
#include <winsock.h>
#include "MiningTools.h"
#include "SampledTrack.h"
#include "TrackSimplify.h"
class Main
{
public:
	Main();
	~Main();
	static vector<vector<Track>> targetsFreqTracks(vector<char*> targets, vector<Track> historyTracks, vector<double*>edges, double prec);
	static DBSCAN analyzeTargetTracks(vector<Track>&targetTracks, double*, double prec, vector<Segment> &segments);
	static void trackSampling(vector<Track>, int startLevel, int endLevel);
	static int insertFreqRes(vector<vector<Track>>, string );
	static void miningInit();
	static void getTrackMain(vector<Track>&, int argc, char* argv[]);
};


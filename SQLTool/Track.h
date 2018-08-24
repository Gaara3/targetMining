#pragma once

#include <vector>
#include<string>
#include "TrackPoint.h"
#include "SqlTool.h"
#include "Grid.h"
#include "Segment.h"

using std::vector;
using std::string;

class Track
{
private:
	int TRACKID;
	int POINTAMOUNT;
	//char* TARGETID;
	string TARGETID;
	/*char* TARGETMODELNUMBER;
	char* TARGETNAME;
	char* TARGETTYPE;*/
	int STARTTIME;
	int ENDTIME;
	double CONFIDENCELEVEL;
	char* SOURCE;
	char* TASKINFO;
	char* OPERATOR;
	char* RESERVE1;
	char* RESERVE2;
	double length;
	
	static char* datetimeConvertor(int input);
	
public:
	vector<TrackPoint> historyPoint;
	vector<int> featurePointIndex;	
	vector<int> mdlPointIndex;

	Track();
	Track(int trackID, char* TARGETID, char* SOURCE, char* TASKINFO, char* OPERATOR,int STARTTIME);
	Track(int trackID, char* TARGETID, char* SOURCE, char* TASKINFO, char* OPERATOR,char* STARTTIME);
	Track(char* trackID, char* POINTAMOUNT, char* TARGETID, char* STARTTIME, char* ENDTIME, char* LENGTH, char* SOURCE, char* TASKINFO, char* CONFIDENCELEVEL, char* OPERATOR, char* RESERVE1, char* RESERVE2);

	~Track();
	const static char* getTargetsQuery;
	static char* getTargetRecords(char* targetID);
	//static Track frequentTrackGenerate(vector<Segment>clusterSegs, vector<Point> freqPoints);
	//static int getMinTrackID();

	int getTrackID();
	int getEndTime();
	int getStartTime();
	int getPointAmount();
	void setPointAmount(int pointAmount);
	void setStartTime(int startTime);
	void setEndTime(int endTime);
	void setTrackID(int trackID);
	void setLength(double length);
	void setTargetID(string id);
	void setTrackIDofPoint(int trackID);
	void insertHisDetail();
	string getTargetID();
	string insertHisSQL();
	char* insertFreqSQL(string Operator);
	void trackEndProcession(int endTime,int pointAmount,vector<TrackPoint>details,double totalLength);
	void extractNnPoint(double* edges,double prec);
	int NnPointOfGrid(int index1, int index2);
	void MDLExtract();
	double MDL_par(int starIdx, int curIdx);
	double lth(int star_index, int cur_index);
	double MDL_nopar(int starIdx, int endIdx);
	void segGenerate(vector<Segment> &segs,int counter);
	void getTrackDetail();
	void getTrackEdges(double* &tmpEdges);
};


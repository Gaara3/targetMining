#include "stdafx.h"
#include "TrackSimplify.h"
#include <math.h>


TrackSimplify::TrackSimplify()
{
}


TrackSimplify::~TrackSimplify()
{
}

double getSquareDist(TrackPoint a, TrackPoint b) {
	double dx = a.CENTERLONGITUDE - b.CENTERLONGITUDE;
	double dy = a.CENTERLATITUDE - b.CENTERLATITUDE;
	return dx * dx + dy * dy;
}

SampledTrack TrackSimplify::simplify(Track track, int level,double radius)
{
	//if(track.getPointAmount()<=2)
	//	return SampledTrack(track,level);

	double tolerance = getTolerance(level, radius);
	double sqTolerance = tolerance * tolerance;
	SampledTrack res = similifyRadialDistance(track, sqTolerance);
	res.setSampleLevel(level);
	res.setPointAmount(res.historyPoint.size());
	return res;
}

double TrackSimplify::getTolerance(int level,double radius)
{
	double resolution = 360 / (256 * pow(2, level));
	return radius * resolution;
}

SampledTrack TrackSimplify::similifyRadialDistance(Track t, double sqTolerance)
{
	SampledTrack res;
	vector<TrackPoint> tmp = t.historyPoint;
	vector<TrackPoint> tmpTrackDetail;
	int prevIdx= 0;
	int newOrder = 1;
	
	//res.originalOrderNumber.push_back(1);
	tmp[0].setOriginOrderNumber(1);
	tmp[0].setOrderNumber(newOrder++);
	tmpTrackDetail.push_back(tmp[0]);

	int tmpIdx =1;
	
	int pointAmount = t.getPointAmount();
	//注意：orderNumber是从1开始计数
	for (; tmpIdx < pointAmount; ++tmpIdx) {
		if (getSquareDist(tmp[tmpIdx], tmp[prevIdx]) > sqTolerance) {
			//res.originalOrderNumber.push_back(tmpIdx+1);
			tmp[tmpIdx].setOriginOrderNumber(tmpIdx + 1);
			tmp[tmpIdx].setOrderNumber(newOrder++);
			tmpTrackDetail.push_back(tmp[tmpIdx]);
			prevIdx = tmpIdx;
		}
	}
	if (prevIdx != tmpIdx-1) {
		//res.originalOrderNumber.push_back(tmpIdx);
		tmp[tmpIdx - 1].setOriginOrderNumber(tmpIdx);
		tmp[tmpIdx - 1].setOrderNumber(newOrder);
		tmpTrackDetail.push_back(tmp[tmpIdx - 1]);
	}
			
	
	res.historyPoint.assign(tmpTrackDetail.begin(), tmpTrackDetail.end());
	return res;
}

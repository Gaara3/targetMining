#include "stdafx.h"
#include "SampledTrack.h"
#include "Processor.h"
#include "SqlTool.h"


void SampledTrack::setSampleLevel(int level)
{
	this->sampleLevel = level;
}

SampledTrack::SampledTrack()
{
}

SampledTrack::SampledTrack(Track t,int level):Track(t)
{
	this->sampleLevel = level;
}


SampledTrack::~SampledTrack()
{
}

void SampledTrack::insertSampledDetail()
{
	/*int sampledSize = (int)this->historyPoint.size();
	int trackID = historyPoint[0].getTrackID();
	for (int counter = 0; counter < sampledSize;counter++) {
		char* sql = new char[1000];
		sprintf_s(sql, 1000, "insert into m_sampling_historytrack_sub(GUID,TRACKID,ORDERNUMBER,ORIGINORDERNUMBER,TIME,\
		POSIXTIME,SAMPLINGLEVEL,SOURCE,CENTERLONGITUDE,CENTERLATITUDE,CENTERALTITUDE,SPEED,ANGLE,CONFIDENCELEVEL)\
		values(UUID(),%d,%d,%d,'%s',%d,%d,'%s',%lf,%lf,%lf,%lf,%lf,1)",
			trackID,historyPoint[counter].getOrderNumber(),historyPoint[counter].getOriginOrderNumber(),SqlTool::datetimeConvertor(historyPoint[counter].getTime()),
			historyPoint[counter].getTime(),sampleLevel, historyPoint[counter].getSource(), historyPoint[counter].CENTERLONGITUDE,
			historyPoint[counter].CENTERLATITUDE, historyPoint[counter].CENTERALTITUDE, historyPoint[counter].getSpeed(), 
			historyPoint[counter].getAngle());
		SqlTool::insertExcutor(sql);
		delete[]sql;
	}
	mysql_commit(&SqlTool::mysql);*/


	int sampledSize = (int)this->historyPoint.size();
	int trackID = historyPoint[0].getTrackID();
	string head("insert into m_sampling_historytrack_sub (GUID,TRACKID,ORDERNUMBER,ORIGINORDERNUMBER,TIME,\
		POSIXTIME,SAMPLINGLEVEL,SOURCE,CENTERLONGITUDE,CENTERLATITUDE,CENTERALTITUDE,SPEED,ANGLE,CONFIDENCELEVEL) values ");
	string tmp(head);
	int tenCounter = 0;
	for (int counter = 0; counter < sampledSize; ++counter, ++tenCounter) {
		tmp.append(historyPoint[counter].sampledSQL(tenCounter == 0, sampleLevel));
		if (tenCounter == 5) {
			tmp.append(";");
			SqlTool::insertExcutor(tmp.data());
			tenCounter = -1;
			tmp = head;
		}
	}
	if (tenCounter != 0) {
		tmp += ";";
		SqlTool::insertExcutor(tmp.data());
	}
	mysql_commit(&SqlTool::mysql);
}

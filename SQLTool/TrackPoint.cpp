#include "stdafx.h"
#include "TrackPoint.h"
#include <string>
#include "Track.h"
#include "SqlTool.h"

using std::to_string;


int TrackPoint::intervalThreshold = 3600*6;

TrackPoint::TrackPoint()
{
}

TrackPoint::TrackPoint(/*char* TARGETID,*/char* POSIXTIME,char* SOURCE,double LONGITUDE,double LATITUDE, double ALTITUDE,char* OPERATOR, char* RESERVE1,char* RESERVE2,double speed,double angle ) {
	//this->TARGETID = TARGETID;
	this->TIME = atoi(POSIXTIME);
	this->SOURCE = SOURCE;
	this->ORDERNUMBER = 1;
	this->CONFIDENCELEVEL = 1;

	this->CENTERLONGITUDE = LONGITUDE;
	this->UPPERLEFTLONGITUDE = LONGITUDE;
	this->UPPERRIGHTLONGITUDE = LONGITUDE;
	this->LOWERLEFTLONGITUDE = LONGITUDE;
	this->LOWERRIGHTLONGITUDE = LONGITUDE;

	this->CENTERALTITUDE = ALTITUDE;
	this->UPPERLEFTALTITUDE = ALTITUDE;
	this->UPPERRIGHTALTITUDE = ALTITUDE;
	this->LOWERLEFTALTITUDE = ALTITUDE;
	this->LOWERRIGHTALTITUDE = ALTITUDE;

	this->CENTERLATITUDE = LATITUDE;
	this->UPPERLEFTLATITUDE = LATITUDE;
	this->UPPERRIGHTLATITUDE = LATITUDE;
	this->LOWERLEFTLATITUDE = LATITUDE;
	this->LOWERRIGHTLATITUDE = LATITUDE;

	this->OPERATOR = OPERATOR;
	this->RESERVE1 = RESERVE1;
	this->RESERVE2 = RESERVE2;
	this->speed = speed;
	this->angle = angle;
}

TrackPoint::TrackPoint(int trackID, char* orderNumber, char* posixTime, char* source, char* centerLONGITUDE, char* centerLatitude, char* centerAltitude, char* speed, char* angle, char* confidenceLevel, char* reserve1, char* reserve2) {
	this->TRACKID = trackID;
	this->ORDERNUMBER = atoi(orderNumber);
	this->TIME = atoi(posixTime);
	this->SOURCE = source;
	this->CENTERLONGITUDE = atof(centerLONGITUDE);
	this->CENTERLATITUDE = atof(centerLatitude);
	this->CENTERALTITUDE = atof(centerAltitude);
	this->speed = atof(speed);
	this->angle = atof(angle);
	//this->CONFIDENCELEVEL = atof(confidenceLevel);
	//this->RESERVE1 = reserve1;
	//this->RESERVE2 = reserve2;

	this->projX = CENTERLONGITUDE * 6731.39 * cos(CENTERLATITUDE) /1000.f;  //等距离圆柱投影
	this->projY = CENTERLATITUDE * 6731.39 /1000.f;
}

void TrackPoint::setGridX(int X)
{
	this->gridX = X;
}

void TrackPoint::setGridY(int Y)
{
	this->gridY = Y;
}

void TrackPoint::setOriginOrderNumber(int n)
{
	this->ORIGINORDERNUMBER = n;
}

int TrackPoint::getOriginOrderNumber()
{
	return this->ORIGINORDERNUMBER;
}

double TrackPoint::getProjX()
{
	return this->projX;
}

double TrackPoint::getProjY()
{
	return this->projY;
}


TrackPoint::~TrackPoint()
{
}

TrackPoint::TrackPoint(int orderNumber, double Longitude, double Latitude):ORDERNUMBER(orderNumber)
{
	this->CENTERLONGITUDE = Longitude;
	this->UPPERLEFTLONGITUDE = Longitude;
	this->UPPERRIGHTLONGITUDE = Longitude;
	this->LOWERLEFTLONGITUDE = Longitude;
	this->LOWERRIGHTLONGITUDE = Longitude;


	this->CENTERLATITUDE = Latitude;
	this->UPPERLEFTLATITUDE = Latitude;
	this->UPPERRIGHTLATITUDE = Latitude;
	this->LOWERLEFTLATITUDE = Latitude;
	this->LOWERRIGHTLATITUDE = Latitude;
}

char* TrackPoint::insertHisDetail(bool first) {
	char* res = new char[512];
	const char* sql = first ? "(UUID(),%d,%d,%d,'%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,1,'%s','%s')" : ",(UUID(),%d,%d,%d,'%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,1,'%s','%s')";
	sprintf_s(res, 512, sql, TRACKID, ORDERNUMBER, TIME, SqlTool::datetimeConvertor(TIME), SOURCE, UPPERLEFTLONGITUDE, UPPERLEFTLATITUDE, UPPERLEFTALTITUDE,
		UPPERRIGHTLONGITUDE, UPPERRIGHTLATITUDE, UPPERRIGHTALTITUDE, LOWERRIGHTLONGITUDE, LOWERRIGHTLATITUDE, LOWERRIGHTALTITUDE,
		LOWERLEFTLONGITUDE, LOWERLEFTLATITUDE, LOWERLEFTALTITUDE, CENTERLONGITUDE, CENTERLATITUDE, CENTERALTITUDE, speed, angle, RESERVE1, RESERVE2);

	return res;
}

char * TrackPoint::sampledSQL(bool first, int sampleLevel)
{
	char* res = new char[256];
	const char* sql = first ? "(UUID(), %d, %d, %d, '%s', %d, %d, '%s', %lf, %lf, %lf, %lf, %lf, 1)" : ",(UUID(), %d, %d, %d, '%s', %d, %d, '%s', %lf, %lf, %lf, %lf, %lf, 1)";
	sprintf_s(res, 256, sql,
		TRACKID, ORDERNUMBER, ORIGINORDERNUMBER, SqlTool::datetimeConvertor(TIME), TIME, sampleLevel, SOURCE, CENTERLONGITUDE,
		CENTERLATITUDE, CENTERALTITUDE, speed, angle, RESERVE1, RESERVE2);
	return res;
}
char* TrackPoint::insertHisSQL() {
	char* res = new char[1500];
	sprintf_s(res, 1500, "insert into m_historytrack_sub(GUID,TRACKID,ORDERNUMBER,POSIXTIME,TIME,SOURCE,UPPERLEFTLONGITUDE,\
	UPPERLEFTLATITUDE,UPPERLEFTALTITUDE, UPPERRIGHTLONGITUDE,UPPERRIGHTLATITUDE,UPPERRIGHTALTITUDE,LOWERRIGHTLONGITUDE,LOWERRIGHTLATITUDE,LOWERRIGHTALTITUDE,\
	LOWERLEFTLONGITUDE,LOWERLEFTLATITUDE,LOWERLEFTALTITUDE,CENTERLONGITUDE,CENTERLATITUDE,\
	CENTERALTITUDE,SPEED,ANGLE,CONFIDENCELEVEL,RESERVE1,RESERVE2) \
	values(UUID(),%d,%d,%d,'%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,1,'%s','%s')",
	TRACKID, ORDERNUMBER, TIME, SqlTool::datetimeConvertor(this->TIME), SOURCE, UPPERLEFTLONGITUDE, UPPERLEFTLATITUDE, UPPERLEFTALTITUDE,
	UPPERRIGHTLONGITUDE, UPPERRIGHTLATITUDE, UPPERRIGHTALTITUDE, LOWERRIGHTLONGITUDE, LOWERRIGHTLATITUDE, LOWERRIGHTALTITUDE,
	LOWERLEFTLONGITUDE, LOWERLEFTLATITUDE, LOWERLEFTALTITUDE, CENTERLONGITUDE, CENTERLATITUDE, CENTERALTITUDE, speed, angle, RESERVE1, RESERVE2);

	return res;	
}

char * TrackPoint::insertFreqSQL()
{
	char* res = new char[1000];
	sprintf_s(res, 1000, "insert into m_selectedfrequentlytrack_sub(GUID,TRACKID,ORDERNUMBER,CENTERLONGITUDE,CENTERLATITUDE,CENTERALTITUDE,UPPERLEFTLONGITUDE,UPPERLEFTLATITUDE,UPPERLEFTALTITUDE,\
	UPPERRIGHTLONGITUDE,UPPERRIGHTLATITUDE,UPPERRIGHTALTITUDE,LOWERRIGHTLONGITUDE,LOWERRIGHTLATITUDE,LOWERRIGHTALTITUDE,LOWERLEFTLONGITUDE,LOWERLEFTLATITUDE,LOWERLEFTALTITUDE,\
CONFIDENCELEVEL,AVGSPEED) VALUES(UUID(),%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,1,%lf);", TRACKID, ORDERNUMBER, CENTERLONGITUDE, CENTERLATITUDE, CENTERALTITUDE, UPPERLEFTLONGITUDE, UPPERLEFTLATITUDE, UPPERLEFTALTITUDE,
UPPERRIGHTLONGITUDE, UPPERRIGHTLATITUDE, UPPERRIGHTALTITUDE, LOWERRIGHTLONGITUDE, LOWERRIGHTLATITUDE, LOWERRIGHTALTITUDE, LOWERLEFTLONGITUDE, LOWERLEFTLATITUDE, LOWERLEFTALTITUDE,speed);
	return res;
}

void  TrackPoint::setOrderNumber(int orderNumber) {
	this->ORDERNUMBER = orderNumber;
}

int TrackPoint::getOrderNumber()
{
	return this->ORDERNUMBER;
}

void TrackPoint::setTrackID(int trackID) {
	this->TRACKID = trackID;
}

int TrackPoint::getTrackID()
{
	return this->TRACKID;
}

int TrackPoint::getTime() {
	return this->TIME;
}

//char* TrackPoint::getTargetID() {
//	return this->TARGETID;
//}

//void Track::setTargetID(char* targetid) {
//	this->TARGETID = targetid;
//}
char* TrackPoint::getSource() {
	return this->SOURCE;
}

char* TrackPoint::getOperator() {
	return this->OPERATOR;
}

double TrackPoint::getSpeed()
{
	return this->speed;
}

void TrackPoint::setAngle(double a)
{
	this->angle = a;
}

double TrackPoint::getAngle() {
	return this->angle;
}

void TrackPoint::setSpeed(double speed)
{
	this->speed = speed;
}

//与上一点时间间隔大，视为起点
bool TrackPoint::headOfTrack(int lastPosixTime) {
	return this->TIME - lastPosixTime > intervalThreshold;
}

char* TrackPoint::datetimeConvertor(int input) {
	time_t t = input;
	tm *ltm = localtime(&t);
	char* res = new char[19];
	sprintf(res, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
	return res;
}

int TrackPoint::getGridX(double*  edges,double prec) {
	return floor((this->CENTERLONGITUDE- edges[1]) / prec);
}

int TrackPoint::getGridY(double* edges, double prec) {
	return floor((this->CENTERLATITUDE - edges[3]) / prec);
}

bool TrackPoint::inTheGrid(Grid grid,double* edges,double prec) {
	return this->getGridX(edges,prec) == grid.gridX  &&  this->getGridY(edges, prec) == grid.gridY;
}
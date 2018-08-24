#pragma once
#include <string>
#include <time.h>
#include "Grid.h"
#include <vector>

using std::string;
using std::vector;

class TrackPoint
{
	private:
		int TRACKID;
		int ORDERNUMBER;
		int ORIGINORDERNUMBER;
		//char* TARGETID;
		int TIME;
		char* SOURCE;
		double UPPERLEFTLONGITUDE;
		double UPPERLEFTLATITUDE;
		double UPPERLEFTALTITUDE;
		double UPPERRIGHTLONGITUDE;
		double UPPERRIGHTLATITUDE;
		double UPPERRIGHTALTITUDE;
		double LOWERLEFTLONGITUDE;
		double LOWERLEFTLATITUDE;
		double LOWERLEFTALTITUDE;
		double LOWERRIGHTLONGITUDE;
		double LOWERRIGHTLATITUDE;
		double LOWERRIGHTALTITUDE;
		double speed;
		double angle;
		double CONFIDENCELEVEL;
		char* OPERATOR;
		char* RESERVE1;
		char* RESERVE2;
		double projX;
		double projY;

		static int intervalThreshold;

		
	public:
		int gridX;	//出于效率考虑直接置为public
		int gridY;
		double CENTERLONGITUDE;//考虑到计算效率，经纬海拔放到public部分以便直接访问
		double CENTERLATITUDE;
		double CENTERALTITUDE;
		char* insertHisSQL();
		char* insertFreqSQL();
		char* insertHisDetail(bool);
		char* sampledSQL(bool first, int sampleLevel);
		TrackPoint();
		~TrackPoint();
		TrackPoint(int orderNumber, double Longitude, double Latitude);
		TrackPoint(/*char* TARGET,*/char* POSIXTIME,char* SOURCE,double LONGITUDE,double LATITUDE,double ALTITUDE, char* OPERATOR,char* RESERVE1,char* RESERVE2,double speed,double angle);
		TrackPoint(int trackID, char* orderNumber, char* posixTime, char* source, char* centerLONGITUDE, char* centerLatitude, char* centerAltitude, char* speed, char* angle, char* confidenceLevel, char* reserve1, char* reserve2);
		void setGridX(int X);
		void setGridY(int Y);
		void setOriginOrderNumber(int);
		int getOriginOrderNumber();
		double getProjX();
		double getProjY();
		bool inTheGrid(Grid grid,double* edges,double prec);
		
		//jUST FOR TEST
		//HistoryTrackDetail(string TRACKID, int ORDERNUMBER, string TARGETID, int TIME, string SOURCE,
		//	double CENTERLONGITUDE, double CENTERLATITUDE, double CENTERALTITUDE,/*double LOWERRIGHTLONGITUDE, double LOWERRIGHTLATITUDE,
		//	double LOWERRIGHTALTITUDE, double CENTERLONGITUDE, double CENTERLATITUDE, double CENTERALTITUDE,*/double CONFIDENCELEVEL,
		//	string OPERATOR, string RESERVE1, string RESERVE2);

		void setOrderNumber(int orderNumber);
		int getOrderNumber();
		void setTrackID(int trackID);
		int getTrackID();
		bool headOfTrack(int lastPosixTime);
		static char* datetimeConvertor(int input);
		int getTime();
		//char* getTargetID();
		char* getSource();
		char* getOperator();
		double getSpeed();
		double getAngle();
		void setAngle(double);
		void setSpeed(double);
		int getGridY(double* edges, double prec);
		int getGridX(double* edges, double prec);
};


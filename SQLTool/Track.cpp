#include "stdafx.h"
#include "Track.h"
#include "MiningTools.h"
#include <string>


using std::string;
using std::to_string;



const char* Track::getTargetsQuery = "SELECT distinct TARGETID FROM m_selectedtrack_main";

//extern vector<Track> HistoryTracks;

char* Track::getTargetRecords(char* targetID) {
	char* res = new char[800];
	sprintf(res, "SELECT TARGETID,POSIXTIME,SOURCE,LONGITUDE,LATITUDE,ALTITUDE,OPERATOR,RESERVE1,RESERVE2,TASKINFO,SPEED,ANGLE FROM m_preprocessing WHERE TARGETID = '%s' ORDER BY POSIXTIME; ", targetID);
	return res;
	//return string("SELECT TARGETID,POSIXTIME,SOURCE,LONGITUDE,LATITUDE,ALTITUDE,OPERATOR,RESERVE1,RESERVE2,TASKINFO FROM preprocessing_copy WHERE TARGETID = '").append(targetID).append("'ORDER BY POSIXTIME; ");
	}


Track::Track()
{
}

Track::Track(int trackID,char* TARGETID, char* SOURCE, char* TASKINFO, char* OPERATOR,int STARTTIME) {
	this->TRACKID = trackID;
	this->TARGETID = TARGETID;
	this->SOURCE = SOURCE;
	this->TASKINFO = TASKINFO;
	this->OPERATOR = OPERATOR;
	this->POINTAMOUNT = 1;
	this->STARTTIME = STARTTIME;
	this->CONFIDENCELEVEL = 1;
}

Track::Track(int trackID, char * TARGETID, char * SOURCE, char * TASKINFO, char * OPERATOR, char * STARTTIME)
{
	this->TRACKID = trackID;
	this->TARGETID = TARGETID;
	this->SOURCE = SOURCE;
	this->TASKINFO = TASKINFO;
	this->OPERATOR = OPERATOR;
	this->POINTAMOUNT = 1;
	this->STARTTIME = atoi(STARTTIME);
	this->CONFIDENCELEVEL = 1;
}


Track::Track(char * trackID, char * POINTAMOUNT, char * TARGETID, char * STARTTIME, char * ENDTIME, char * LENGTH, char * SOURCE, char * TASKINFO, char * CONFIDENCELEVEL, char * OPERATOR, char * RESERVE1, char * RESERVE2)
	{
		this->TRACKID = atoi(trackID);
		this->POINTAMOUNT = atoi(POINTAMOUNT);
		this->TARGETID = TARGETID;
		this->STARTTIME = atoi(STARTTIME);
		this->ENDTIME = atoi(ENDTIME);
		this->length = atof(LENGTH);
		this->SOURCE = SOURCE;
		this->TASKINFO = TASKINFO;
		this->CONFIDENCELEVEL = atof(CONFIDENCELEVEL);
		this->RESERVE1 = RESERVE1;
		this->RESERVE2 = RESERVE2;
}

int Track::getTrackID() {
	return this->TRACKID;
}


void Track::setEndTime(int endTime) {
	this->ENDTIME = endTime;
}

int Track::getEndTime()
{
	return this->ENDTIME;
}

int Track::getStartTime()
{
	return this->STARTTIME;
}

int Track::getPointAmount()
{
	return this->POINTAMOUNT;
}

void Track::setPointAmount(int pointAmount) {
	this->POINTAMOUNT = pointAmount;
}

void Track::setStartTime(int startTime)
{
	this->STARTTIME = startTime;
}

void Track::setTrackID(int trackID) {
	this->TRACKID = trackID;
}

void Track::setTrackIDofPoint(int trackID) {
	for (vector<TrackPoint>::iterator i = this->historyPoint.begin(); i!=historyPoint.end();i++) {
		(*i).setTrackID(trackID);
	}
}
void Track::insertHisDetail()
{
	char *trackSql = new char[1000];
	sprintf_s(trackSql, 1000, "insert into m_historytrack_main (GUID,TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,SOURCE,TASKINFO,CONFIDENCELEVEL,OPERATOR,LENGTH)\
 values(UUID(),%d,%d,'%s','%s','%s','%s','%s',%lf,'%s',%lf)", TRACKID, POINTAMOUNT, TARGETID, SqlTool::datetimeConvertor(STARTTIME), SqlTool::datetimeConvertor(ENDTIME), SOURCE, TASKINFO,
		CONFIDENCELEVEL, OPERATOR, length);
	SqlTool::insertExcutor(trackSql);
	delete[] trackSql;
	int tenCounter = 0;
	string head("insert into m_historytrack_sub(GUID,TRACKID,ORDERNUMBER,POSIXTIME,TIME,SOURCE,UPPERLEFTLONGITUDE,\
	UPPERLEFTLATITUDE,UPPERLEFTALTITUDE, UPPERRIGHTLONGITUDE,UPPERRIGHTLATITUDE,UPPERRIGHTALTITUDE,LOWERRIGHTLONGITUDE,LOWERRIGHTLATITUDE,LOWERRIGHTALTITUDE,\
	LOWERLEFTLONGITUDE,LOWERLEFTLATITUDE,LOWERLEFTALTITUDE,CENTERLONGITUDE,CENTERLATITUDE,\
	CENTERALTITUDE,SPEED,ANGLE,CONFIDENCELEVEL,RESERVE1,RESERVE2) \
	values ");
	string tmp(head);
	for (TrackPoint p : historyPoint) {
		tmp.append(p.insertHisDetail(tenCounter == 0));
		if (tenCounter == 9) {
			tmp.append(";");
			SqlTool::insertExcutor(tmp.data());
			tenCounter = -1;
			tmp = head;
		}
		++tenCounter;
	}
	if (tenCounter != 0) {
		tmp.append(";");
		SqlTool::insertExcutor(tmp.data());
	}
	mysql_commit(&SqlTool::mysql);
}
void Track::setLength(double length){
	this->length = length;
}

void Track::setTargetID(string id)
{
	this->TARGETID = id;
}

string Track::getTargetID()
{
	return TARGETID;
}

string Track::insertHisSQL() {
	
	string insertSql = "insert into m_historytrack_main (GUID,TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,SOURCE,TASKINFO,CONFIDENCELEVEL,OPERATOR,LENGTH) values(";
	insertSql.append("UUID(),").append(to_string(this->TRACKID)).append(",").append(to_string(POINTAMOUNT)).append(",'").append(this->TARGETID).append("','")
		.append(SqlTool::datetimeConvertor(this->STARTTIME)).append("','").append(SqlTool::datetimeConvertor(this->ENDTIME)).append("','").append(this->SOURCE).append("','").append(this->TASKINFO)
		.append("',").append(to_string(this->CONFIDENCELEVEL)).append(",'").append(this->OPERATOR).append("',").append(to_string(this->length)).append(");");


	//char* res = new char[800];
	//sprintf(res, "insert into m_historytrack_main(GUID, TRACKID, POINTAMOUNT, TARGETID, STARTTIME, ENDTIME, SOURCE, TASKINFO, CONFIDENCELEVEL, OPERATOR, RESERVE1, RESERVE2) values(UUID(),\
	//	'%s',%d,'%s','%s','%s','%s','%s',%lf,'%s','%s','%s');",TRACKID,POINTAMOUNT,TARGETID,datetimeConvertor(STARTTIME),datetimeConvertor(ENDTIME),SOURCE,TASKINFO,
	//	CONFIDENCELEVEL,OPERATOR,RESERVE1,RESERVE2);

	//const char* res = "insert into M_HISTORYTRACK_MAIN(GUID, TRACKID, POINTAMOUNT, TARGETID, STARTTIME, ENDTIME, SOURCE, TASKINFO, CONFIDENCELEVEL, OPERATOR, RESERVE1, RESERVE2) values(UUID(), 1, 2,3, '2018-05-30 14:14:14', '2018-5-3', 'source', 'taskInfo', 0.000000, 'Gaara', '', '');";
	return insertSql;
}

char * Track::insertFreqSQL(string Operator)
{
	char res[1000];
	sprintf_s(res,1000, "insert into m_selectedfrequentlytrack_main (GUID,TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,LENGTH,CONFIDENCELEVEL,OPERATOR) VALUES\
(UUID(),%d,%d,%s,'%s','%s',%lf,1,'%s');", TRACKID, POINTAMOUNT,TARGETID.data(), SqlTool::datetimeConvertor(this->STARTTIME), SqlTool::datetimeConvertor(this->ENDTIME),length,Operator.data());
	return res;
}

char* Track::datetimeConvertor(int input) {
	time_t t = input;
	tm *ltm = localtime(&t);
	char* res = new char[19];
	sprintf(res, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
	return res;
}

void Track::trackEndProcession(int endTime, int pointAmount, vector<TrackPoint>details,double totallength) {
	this->setEndTime(endTime);
	this->setPointAmount(pointAmount);
	this->setLength(totallength);
	this->historyPoint.assign(details.begin(), details.end());
}

int Track::NnPointOfGrid(int index1, int index2) {//先算出网格平均坐标，再得出最近点
	if (index1 == index2)
		return index1;
	double tmpLongitude = 0, tmpLatitude = 0;
	for (int counter = index1; counter <= index2; counter++) {
		tmpLatitude += historyPoint[counter].CENTERLATITUDE;
		tmpLongitude += historyPoint[counter].CENTERLONGITUDE;
	}
	tmpLatitude /= (index2 - index1 + 1);
	tmpLongitude /= (index2 - index1 + 1);
	int resIndex = -1;
	double minBias = 800;//与网格均值点的最小偏移。处于计算量考虑，暂时用经纬度偏移量代替（若误差较大则改为实际距离）
	for (int counter = index1; counter <= index2; counter++) {
		double tmpBias = abs(historyPoint[counter].CENTERLATITUDE - tmpLatitude) + abs(historyPoint[counter].CENTERLONGITUDE - tmpLongitude);
		if (tmpBias < minBias) {
			minBias = tmpBias;
			resIndex = counter;
		}
	}
	return resIndex;
}

void Track::extractNnPoint(double* edges, double prec) {
	Grid curGrid = { true,-1,-1,0,-1 };
	for (int counter = 0; counter < this->POINTAMOUNT; counter++) {//TODO  考虑最后一个点！
		int tmpGridX = historyPoint[counter].getGridX(edges, prec), tmpGridY = historyPoint[counter].getGridY(edges, prec);
		historyPoint[counter].gridX = tmpGridX;
		historyPoint[counter].gridY = tmpGridY;		//出于聚类距离考虑，暂时不适用放大坐标，直接用经纬信息
		/*historyPoint[counter].gridX = historyPoint[counter].CENTERLONGITUDE;
		historyPoint[counter].gridY = historyPoint[counter].CENTERLATITUDE;*/
		//if 新格:计算目前的NnPoint,更新startIndex      旧格:更新endIndex
		if (curGrid.inTheGrid(tmpGridX,tmpGridY)) {	//仍处于curGrid，则更新endIndex
			curGrid.endIndex = counter;
		}
		else {
			if(counter!=0)
				this->featurePointIndex.push_back(NnPointOfGrid(curGrid.startIndex, curGrid.endIndex));
			curGrid.startIndex = counter;
			curGrid.endIndex = counter;
			curGrid.gridX = tmpGridX;
			curGrid.gridY = tmpGridY;
		}
	}
	this->featurePointIndex.push_back(NnPointOfGrid(curGrid.startIndex, curGrid.endIndex));
}

void Track::MDLExtract() {
	int star_index = 0, length = 1,curr_index = 0;
	int len = (int)this->historyPoint.size();
	mdlPointIndex.push_back(0);
	if (len > 1) {	
		while (star_index + length < len) {
			curr_index = star_index + length;

			double cost_par,cost_nopar = MDL_nopar(star_index, curr_index);
			if (length == 1)
				cost_par = cost_nopar;
			else
				cost_par = MDL_par(star_index, curr_index);
			if (cost_par > cost_nopar) {
				mdlPointIndex.push_back(curr_index - 1);//TODO  待考察
				star_index = curr_index - 1 ;
				length = 1;
			}
			else {
				length++;
			}
		}
		mdlPointIndex.push_back(len - 1);
	}	
}

double Track::MDL_par(int star_index, int cur_index) {
	double res = 0;
	//int realStartIdx = featurePointIndex[star_index];
	//int realEndIdx = featurePointIndex[cur_index];
	/*double x1 = historyPoint[realStartIdx].CENTERLONGITUDE;
	double y1 = historyPoint[realStartIdx].CENTERLATITUDE;
	double x2 = historyPoint[realEndIdx].CENTERLONGITUDE;
	double y2 = historyPoint[realEndIdx].CENTERLATITUDE;*/
	double x1 = historyPoint[star_index].getProjX();
	double y1 = historyPoint[star_index].getProjY();
	double x2 = historyPoint[cur_index].getProjX();
	double y2 = historyPoint[cur_index].getProjY();
	res += sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	double weights[3] = { 1,1,0 };//此时当做无水平分量
	Segment s2e = Segment{ Point{x1,y1 },Point{ x2,y2 } };

	for (int counter = star_index; counter < cur_index; counter++) {
		//int tmpIdx1 = featurePointIndex[counter], tmpIdx2 = featurePointIndex[counter + 1];
		Segment temp = Segment{ Point{ historyPoint[counter].getProjX(),historyPoint[counter].getProjY() },Point{ historyPoint[counter+1].getProjX(),historyPoint[counter+1].getProjY() } };
		res += MiningTools::distanceBetweenLines(s2e,temp,weights);
	}
	return res;
}

double Track::MDL_nopar(int star_index, int cur_index) {
	//return lth(star_index,cur_index);
	double res = 0;
	for (int counter = star_index; counter < cur_index; ++counter) {
		double x1 = historyPoint[counter].getProjX(), y1 = historyPoint[counter].getProjY();
		double x2 = historyPoint[counter + 1].getProjX(), y2 = historyPoint[counter + 1].getProjY();
		res += sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	return res;
}

double Track::lth(int star_index, int cur_index) {
	double res = 0;
	for (int counter1 = star_index; counter1 < cur_index; counter1++) {
		for (int counter2 = counter1+1; counter2 <= cur_index; counter2++) {
			int tmpIdx1 = featurePointIndex[counter1], tmpIdx2 = featurePointIndex[counter2];
			int x1 = historyPoint[tmpIdx1].CENTERLONGITUDE;
			int y1 = historyPoint[tmpIdx1].CENTERLATITUDE;
			int x2 = historyPoint[tmpIdx2].CENTERLONGITUDE;
			int y2 = historyPoint[tmpIdx2].CENTERLATITUDE;
			res += sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		}
	}
	return res;
}

void Track::segGenerate(vector<Segment> &segs,int trackIdx) {
	int pointAmount = (int)this->mdlPointIndex.size();
	int segAmount = pointAmount - 1;
	for (int counter = 0; counter < segAmount; counter++) {
		int startIdx = this->mdlPointIndex[counter];
		int endIdx = this->mdlPointIndex[counter + 1];
		TrackPoint start = historyPoint[startIdx];
		TrackPoint end = historyPoint[endIdx ];
		segs.push_back({ Point{start.getProjX(),start.getProjY()},Point{end.getProjX(),end.getProjY() },trackIdx,startIdx ,trackIdx,endIdx });
	}
}

void Track::getTrackDetail()
{
	char* sql = new char[256];
	sprintf_s(sql, 256, "select ordernumber,posixtime,source,centerlongitude,centerlatitude,centeraltitude,speed,angle,confidencelevel,reserve1,reserve2 from m_selectedtrack_sub where TRACKID = %d;", TRACKID);
	SqlTool::operationExcutor(sql, SqlTool::res);

	while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {//需要转化时间戳字符串
		historyPoint.push_back(TrackPoint(TRACKID, SqlTool::column[0],
			SqlTool::column[1], SqlTool::column[2], SqlTool::column[3], SqlTool::column[4], SqlTool::column[5],
			SqlTool::column[6], SqlTool::column[7], SqlTool::column[8], SqlTool::column[9], SqlTool::column[10]));
	}
	mysql_free_result(SqlTool::res);
	setPointAmount((int)historyPoint.size());
}

void Track::getTrackEdges(double* &tmpEdges)
{
	tmpEdges[0] = tmpEdges[1] = historyPoint[0].CENTERLONGITUDE;
	tmpEdges[2] = tmpEdges[3] = historyPoint[0].CENTERLATITUDE;

	for (TrackPoint p : historyPoint) {
		if (p.getProjX() > tmpEdges[0])
			tmpEdges[0] = p.getProjX();
		else if (p.getProjX() < tmpEdges[1])
			tmpEdges[1] = p.getProjX();

		if (p.getProjY() > tmpEdges[2])
			tmpEdges[2] = p.getProjY();
		else if (p.getProjY() < tmpEdges[3])
			tmpEdges[3] = p.getProjY();
	}

}

Track::~Track()
{
}


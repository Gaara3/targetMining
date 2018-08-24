#include "stdafx.h"
#include "Processor.h"
#include "SqlTool.h"
#include <algorithm>    // std::sort
#include "MiningTools.h"

Processor::Processor()
{
}


Processor::~Processor()
{
}

//SqlTool Processor::sqlTool ;
//MYSQL_RES* Processor::res;
//MYSQL_ROW Processor::column;

extern int minPts;
/*
	对入参列表的所有target进行预处理
*/
vector<double*> Processor::targetsPreProcession(vector<char*> targets, vector<Track> &HistoryTracks) {
	int targetsNum = (int)targets.size();
	vector<double*> edges;

	for (int counter = 0; counter < targetsNum; counter++) {
		//printf("********************************new Target********************************\n");
		edges.push_back( Processor::getTargetEdges(targets[counter]));
	}
	/*for (vector<double*>::iterator i = edges.begin(); i != edges.end(); i++) {
		printf("%10g,%10g,%10g,%10g\n", (*i)[0], (*i)[1], (*i)[2], (*i)[3]);
	}*/
	return edges;
}

double* Processor::getTargetEdges(char * targetID)
{
	double* edges = new double[4];
	char maxLongSql[150];
	char minLongSql[150];
	char maxLatSql[150];
	char minLatSql[150];
	sprintf(maxLongSql, "SELECT max(LONGITUDE)from m_preprocessing where targetID = '%s';", targetID);
	sprintf(minLongSql, "SELECT min(LONGITUDE)from m_preprocessing where targetID = '%s';", targetID);
	sprintf(maxLatSql, "SELECT max(LATITUDE)from m_preprocessing where targetID = '%s';", targetID);
	sprintf(minLatSql, "SELECT min(LATITUDE)from m_preprocessing where targetID = '%s';", targetID);
	edges[0] = atof(SqlTool::getVariableFromDB(maxLongSql));
	edges[1] = atof(SqlTool::getVariableFromDB(minLongSql));
	edges[2] = atof(SqlTool::getVariableFromDB(maxLatSql));
	edges[3] = atof(SqlTool::getVariableFromDB(minLatSql));
	return edges;
}

/*
	对特定目标进行预处理
*/
void Processor::oneTargetPreProcession(char* target, vector<Track>&HistoryTracks,bool &newTarget,int &trackID) {

	Track tmp;
	int lastPosixtime = 0;
	int orderNumber = 0;
	double totalLength = 0;
	double lastLongitude = 181, lastLatitude = 91;
	SqlTool::operationExcutor(Track::getTargetRecords(target), SqlTool::res);
	//mysql_autocommit(&SqlTool::mysql, 0);
	MYSQL_RES *targetRecord = SqlTool::res;
	vector<TrackPoint> details;

	while (SqlTool::column = mysql_fetch_row(targetRecord)) {
		pointPreprocession(details, SqlTool::column, HistoryTracks,trackID, lastPosixtime,orderNumber, newTarget,totalLength,lastLongitude,lastLatitude);
		newTarget = false;//TODO   改进
	}
	//单目标最后一段轨迹需要专门处理一次
	HistoryTracks.back().trackEndProcession(lastPosixtime, orderNumber, details,totalLength);
	//SqlTool::insertExcutor(HistoryTracks.back().insertHisSQL().data());
	mysql_commit(&SqlTool::mysql);
}


void Processor::pointPreprocession(vector<TrackPoint>&details,MYSQL_ROW column, vector<Track>&HistoryTracks,int &trackID, int &lastPosixTime,int &orderNumber,bool &newTarget,double &totalLength,double &lastLongitude,double& lastLatitude) {
	
	double longitude = atof(column[3]), latitude = atof(column[4]), altitude = atof(column[5]);
	double speed = atof(column[10]), angle = atof(column[11]);
	TrackPoint point = TrackPoint(/*column[0],*/ column[1], column[2], longitude, latitude, altitude, column[6], column[7], column[8], speed,angle);
	
	if (point.headOfTrack(lastPosixTime)) {//该点是一段新轨迹
		//printf("===============================new track============================\n");
		trackID++;
		if (!newTarget) {//newTarget 已涵盖队列为空的情况，不再另行判断
			Track *lastTrack = &HistoryTracks.back();
			lastTrack->trackEndProcession(lastPosixTime, orderNumber, details,totalLength);
			//SqlTool::insertExcutor(lastTrack->insertHisSQL().data());	//为了不传类别无关变量res，暂不加入封装
			mysql_commit(&SqlTool::mysql);
			//引用值重置，考虑封装
			lastPosixTime = 0;			
			orderNumber = 0;			
			totalLength = 0; lastLatitude = 91; lastLongitude = 181;	//新轨迹开始，设置异常值以便于判断
			details.clear();	
		}
		HistoryTracks.push_back(Track(trackID, column[0], point.getSource(),column[9], point.getOperator(), point.getTime()));	//轨迹数组加入新track
		//details.clear();
	}
	
	//该点本身所需的操作:确定trackID,确定orderNumber，更新lastPosixTime，插入数据库
	point.setTrackID(trackID);
	lastPosixTime = point.getTime();
	point.setOrderNumber(++orderNumber);
	details.push_back(point);

	totalLength += distanceBetweenPoints(lastLongitude, lastLatitude,longitude,latitude);//计算两地点距离
	lastLatitude = latitude;
	lastLongitude = longitude;
	/*char* sql = point.insertHisSQL();
	SqlTool::insertExcutor(sql);
	delete sql;*/
	//printf("                              new point%d                              \n", orderNumber);
}

double Processor::distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude) {
	double res = 0;
	if (lastLatitude <= 90 && lastLongitude <= 180) {	//当一段轨迹完结，设置异常值以便于新一段计算
		double p = 0.017453292519943295;    // Math.PI / 180
		double a = 0.5 - cos((latitude - lastLatitude) * p) / 2 +cos(latitude * p) * cos(lastLatitude * p) *(0.5 - cos((longitude - lastLongitude) * p)/2);

		res= 12742 * asin(sqrt(a)); // 2 * R; R = 6371 km
	}
	return res;
}

void Processor::tracksExtract(vector<Track> &tracks,double* edges,double prec){
	int trackNum = (int)tracks.size();
	/*for (int counter = 0; counter < trackNum; counter++) {
		tracks[counter].extractNnPoint(edges,prec);
	}*/
	for (int counter = 0; counter < trackNum; ++counter) {
		int pNum = tracks[counter].historyPoint.size();
		for (int j = 0; j < pNum; ++j)
			tracks[counter].featurePointIndex.push_back(j);
	}
}

void Processor::tracksMDL(vector<Track> &tracks) {
	int trackNum = (int)tracks.size();
	for (int counter = 0; counter < trackNum; counter++) {
		tracks[counter].MDLExtract();
	}
}

vector<Segment> Processor::tracks2Segment(vector<Track>& tracks, vector<Segment> &result)
{
	int trackNum = (int)tracks.size();
	
	for (int counter = 0; counter < trackNum; counter++) {
		tracks[counter].segGenerate(result,counter);
	}
	return result;
}

double** Processor::disMatrice(vector<Segment>segs)
{
	int size = (int)segs.size();
	double **disMat = new double*[size];
	bool** matFlag = new bool*[size];
	double weights[3] = { 0.5,1,2 };
	for (int counter = 0; counter < size; counter++) {
		disMat[counter] = new double[size];
		matFlag[counter] = new bool[size];
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++)
			matFlag[x][y] = false;
	}
	for (int xCounter = 0; xCounter < size; xCounter++) {
		for (int yCounter = 0; yCounter < size; yCounter++) {
			if (matFlag[xCounter][yCounter])	//该位置的值计算过，则略过
				continue;
			disMat[xCounter][yCounter] = xCounter==yCounter?0:MiningTools::distanceBetweenLines(segs[xCounter], segs[yCounter], weights);
			disMat[yCounter][xCounter] = disMat[xCounter][yCounter];
			matFlag[xCounter][yCounter] = true;
			matFlag[yCounter][xCounter] = true;
		}
	}
	for (int counter = 0; counter < size; counter++)
		delete[] matFlag[counter];
	delete[] matFlag;
	return disMat;
}

void Processor::processByTarget(vector<Track> historyTracks)
{
	int trackNum = (int)historyTracks.size();
	string curTar = historyTracks[0].getTargetID();
	for (int counter = 0; counter < trackNum; counter++) {
		if (curTar==historyTracks[counter].getTargetID()) {

		}
	}
}

/*
1.计算平均向量
2.簇旋转
3.延x轴扫描，寻找多段的平均点，形成频繁轨迹的旋转后坐标
4.逆向旋转，得出频繁轨迹真实坐标
5.根据簇内点信息，生成频繁项真实信息
*/
vector<Track> Processor::clusterAnalyze(vector<Track> targetTracks,vector<Segment> segs, vector<int>* clusterInfo,int clusterNum,int &trackID)
{
	//clusters分解
	vector<vector<Segment>>	clusters;
	vector<Track> freqTracks;
	for (int counter = 0; counter < clusterNum; counter++) {//对每个簇进行初始化赋值
		vector<Segment> clusterDetail;
		vector<int>::iterator end = clusterInfo[counter].end();
		for (vector<int>::iterator i = clusterInfo[counter].begin(); i != end; i++) {
			//根据每个簇的信息指针，对指针内容进行遍历，push对应seg信息
			int tmpidx = *i;
			clusterDetail.push_back(segs[tmpidx]);
		}
		clusters.push_back(clusterDetail);
	}	
	for (vector<Segment>c :clusters) {	//对每个簇进行轮循操作
		Vector2D clusterV = Processor::clusterVector(c);
		double cosA = clusterV.x / vectorMag(clusterV);
		double angle = acos(cosA);    //使用x轴单位向量(1,0)算出簇所需的逆时针旋转角
		angle *= (clusterV.y < 0 ? 1 : -1);//传入所需逆时针旋转的角度,根据y值正负决定角度正负
		Processor::clusterRotation(c,angle);
		vector<Point> freqPointInCluster = Processor::clusterScan(c);	//得出簇内的频繁点相对投影坐标
		for (vector<Point>::iterator p = freqPointInCluster.begin(); p != freqPointInCluster.end(); p++) {
			(*p).rotateAnticlockwise(-angle);	//将得出的频繁点坐标转回真实投影坐标
			//此处需要将投影坐标转回真实经纬坐标
		}
		//构建真实轨迹信息(trackID POINTAMOUNT TARGETID STARTTIME ENDTIME LENGTH SOURCE TASKINFO CONFIDENCELEVEL OPERATOR RESERVE1 RESERVE2)
		//构建真实轨迹点信息(trackID ORDERNUMBER TIME POSIXTIME SOURCE 经纬 置信度 AVGSPEED RESERVE1 RESERVE2)
		Track freqTrack = Processor::frequentTrackGenerate(targetTracks, c, freqPointInCluster);
		freqTrack.setTrackID(trackID);
		freqTrack.setTrackIDofPoint(trackID);
		trackID++;
		freqTracks.push_back(freqTrack);
	}
	return freqTracks;
}

/*
1.计算平均向量
2.簇旋转
3.延x轴扫描，寻找多段的平均点，形成频繁轨迹的旋转后坐标
4.逆向旋转，得出频繁轨迹真实坐标
5.根据簇内点信息，生成频繁项真实信息
*/
Vector2D Processor::clusterVector(vector<Segment>clusterSegs)
{
	double totalStartX = 0, totalStartY = 0, totalEndX = 0, totalEndY = 0;
	vector<Segment>::iterator end = clusterSegs.end();
	for (vector<Segment>::iterator i = clusterSegs.begin(); i !=end; i++) {
		totalStartX += (*i).start.x;
		totalStartY += (*i).start.y;
		totalEndX += (*i).end.x;
		totalEndY += (*i).end.y;
	}
	int clusterSize = (double)clusterSegs.size();
	totalStartX /= clusterSize;
	totalEndX /= clusterSize;
	totalStartY /= clusterSize;
	totalEndY /= clusterSize;
	return Vector2D(Point{ totalStartX, totalStartY }, Point{ totalEndX, totalEndY });
}

/*
将簇内线段逆时针旋转
*/
void Processor::clusterRotation(vector<Segment>&segs, double angle)
{
	for (vector<Segment>::iterator i = segs.begin(); i != segs.end(); i++) {
		MiningTools::segmentRotation(*i, angle);
	}
}

void Processor::segmentRotation(Segment &seg, double angle) {
	seg.start.rotateAnticlockwise(angle);
	seg.end.rotateAnticlockwise(angle);
}

/*
传入的是旋转过后的簇，扫描线就是x轴垂线
*/
vector<Point> Processor::clusterScan(vector<Segment>segs)
{
	//对各段起终点x值排序
	//对各x值检验与簇的相交情况
	//交点大于min阈值，则计算均值点
	vector<double> segX;
	vector<Point> frequentPoint;
	for (vector<Segment>::iterator i = segs.begin(); i != segs.end(); i++) {
		segX.push_back((*i).start.x);
		segX.push_back((*i).end.x);
	}
	std::sort(segX.begin(), segX.end());	
	segX.erase(unique(segX.begin(), segX.end()), segX.end());	//去除重复的x坐标
	for (vector<double>::iterator i = segX.begin(); i != segX.end(); i++) {	//对簇内的每个起、终点进行判定
		int intersectCounter = 0;
		for (Segment s : segs) {
			if (s.start.x <= *i  && s.end.x >= *i)
				++intersectCounter;
		}
		if (intersectCounter < minPts)	//交点个数小于阈值，则跳过该点
			continue;
		else {	//交点个数大于阈值，需要求出平均坐标
			double avgy = avgYofCluster(segs, *i);
			Point avgPoint = Point{ *i, avgy };		//得出旋转后的轨迹点
			frequentPoint.push_back(avgPoint);
		}
	}
	return frequentPoint;
}

double Processor::avgYofCluster(vector<Segment>segs,double x)
{
	int totalY = 0, intersectNum = 0;
	for (Segment s : segs) {
		if (s.start.x <= x && s.end.x >= x) {	
			++intersectNum;
			Vector2D s2e = Vector2D(s.start, s.end);
			double alph=(s.end.x - x) / vectorMag(s2e);
			totalY += (s.start.y + alph * s2e.y);
		}			
	}
	totalY /=intersectNum;
	return totalY;
}

Track Processor::frequentTrackGenerate(vector<Track> targetTracks, vector<Segment> clusterSegs, vector<Point> freqPoints)
{
	//构建真实轨迹信息(trackID)
	//构建真实轨迹点信息(trackID)
	int freqPointNum = freqPoints.size();
	Track track;
	int startTime = MAXINT;
	int endTime = 0;
	double avgspeed = 0;
	double length = 0;
	for (Segment s : clusterSegs) {
		int tmpStartTime = targetTracks[s.startT].getStartTime();
		int tmpEndTime = targetTracks[s.endT].getEndTime();
		if (tmpStartTime < startTime)
			startTime = tmpStartTime;
		if (tmpEndTime > endTime)
			endTime = tmpEndTime;
		avgspeed += targetTracks[s.endT].historyPoint[s.endIdx].getSpeed();
	}
	avgspeed /= (clusterSegs.size());
	//TrackPoint(char* TARGET,char* POSIXTIME,char* SOURCE,double LONGITUDE,double LATITUDE,double ALTITUDE, char* OPERATOR,char* RESERVE1,char* RESERVE2);
	for (int counter = 0; counter < freqPointNum; ++counter) {
		TrackPoint p = TrackPoint(counter, freqPoints[counter].x, freqPoints[counter].y);
		p.setSpeed(avgspeed);
		
		if (counter <freqPointNum-1)
			length += (MiningTools::distanceBetweenPoints(freqPoints[counter].x, freqPoints[counter].y, freqPoints[counter + 1].x, freqPoints[counter + 1].y));
		track.historyPoint.push_back(p);
	}//各轨迹点初始化
	track.setTargetID(targetTracks[clusterSegs[0].endT].getTargetID());
	track.setLength(length);
	track.setStartTime(startTime);
	track.setEndTime(endTime);
	track.setPointAmount(freqPointNum);
	return track;
}

void Processor::getTrackProjRange(vector<Track>& targetTracks,double* &edges)
{
	//maxLong,minLong
	edges[0] = edges[1] = targetTracks[0].historyPoint[0].getProjX();
	edges[2] = edges[3] = targetTracks[0].historyPoint[0].getProjY();
	
	for (Track t : targetTracks) {
		double *tmp = new double[4];
		t.getTrackEdges(tmp);
		if (edges[0] < tmp[0])
			edges[0] = tmp[0];
		if (edges[1] > tmp[1])
			edges[1] = tmp[1];
		if (edges[2] < tmp[2])
			edges[2] = tmp[2];
		if (edges[3] > tmp[3])
			edges[3] = tmp[3];
	}		
}

//vector<double*> Processor::getTrackProjRange(vector<char*> targets, vector<Track>& HistoryTracks)
//{
//	vector<double*> res;
//	res.reserve(targets.size());
//	for()
//}

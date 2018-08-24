#include "stdafx.h"
#include "Main.h"
#include "Log.h"
#include <string>

using std::string;

Main::Main()
{
}


Main::~Main()
{
}


double prec = 0.05;
int minPts = 2;

int main(int argc, char* argv[]) {
	SqlTool::dbInit();
	//Main::miningInit();		根据用户选定情况进行挖掘，不需全盘清空

	SL_LOG("Read target info start");
	SqlTool::operationExcutor(string(Track::getTargetsQuery).append(" where operator = '").append(argv[argc-1]).append("'").data(), SqlTool::res);
	vector<char*> targets;
	SL_LOG("Read target info end");

	while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {
		targets.push_back(SqlTool::column[0]);
	}
	//mysql_free_result(SqlTool::res);
	SL_LOG("Read trackMain info start");
	vector<Track> hisTracks;
	Main::getTrackMain(hisTracks, argc, argv);
	SL_LOG("Read trackMain info end");

	SL_LOG("Read track detail info start");
	for (vector<Track>::iterator i = hisTracks.begin(); i != hisTracks.end(); i++) {
		(*i).getTrackDetail();
	}
	SL_LOG("Read track detail info end");

	vector<double*> edges = Processor::targetsPreProcession(targets, hisTracks);//初始轨迹分段(根据时间阈值)，并得出4条边界

	SL_LOG("Mining frequent tracks of targets start");
	vector<vector<Track>> allFreqTracks = Main::targetsFreqTracks(targets, hisTracks,edges,prec);//根据轨迹片段，进行:网格化、特征点提取、生成线段距离矩阵、DBSCAN、聚类结果解析
	SL_LOG("Mining frequent tracks of targets end");

	for (double* p : edges)
		delete[] p;
	edges.clear();
	string Operator = argv[argc - 1];
	SL_LOG("insert frequent tracks start");
	int resCounter = Main::insertFreqRes(allFreqTracks, Operator);
	SL_LOG("insert frequent tracks end");

	printf("{\"resCount\":%d}", resCounter);
	mysql_free_result(SqlTool::res);
	//system("pause");
	return 0;
}

/*
假如需要单独调用，需要先获取目标列表、各目标边界、各目标在select条件下的分段
*/
vector<vector<Track>> Main::targetsFreqTracks(vector<char*> targets, vector<Track> historyTracks,vector<double*>edges,double prec)
{
	int targetNum = (int)targets.size();
	int trackNum = (int)historyTracks.size();
	
	//对每一个目标，选出其轨迹段。以目标为单位进行分析
	vector<vector<Track>> FreqTracks;
	int trackID = 0;
	for (int counter = 0; counter < targetNum; counter++) {
		vector<Track> targetTracks ;
		vector<Segment>targetSegs;
		//printf("target %s:", targets[counter]);

		for (int trackCounter = 0; trackCounter < trackNum; trackCounter++) {
			if (targets[counter] == historyTracks[trackCounter].getTargetID()) {
				targetTracks.push_back(historyTracks[trackCounter]);
			}						
			//else	//轨迹所属目标变化，则跳出，分析当前目标的情况
				//break;								
		}
		if (targetTracks.size() < 1)
			continue;
		double *targetEdge = new double[4];
		Processor::getTrackProjRange(targetTracks, targetEdge);	//得到投影坐标的4个边界极值
		DBSCAN targetDBSCANNER =analyzeTargetTracks(targetTracks, targetEdge,prec, targetSegs);//根据目标数据生成对应的DBSCANER
		vector<int>* clusterInfo = targetDBSCANNER.clusterGenerate();//启动DBSCANER
		int clusterNum = targetDBSCANNER.clusterNum;
		vector<Track> targetFreqTracks = Processor::clusterAnalyze(targetTracks,targetSegs, clusterInfo,clusterNum,trackID);
			
		FreqTracks.push_back(targetFreqTracks);
		//printf("---------------------------------------\n");
	}
	return FreqTracks;
}

DBSCAN Main::analyzeTargetTracks(vector<Track>&targetTracks,double* edge,double prec, vector<Segment> &segments) {

	//Processor::tracksExtract(targetTracks, edge, prec);	//轨迹网格化处理
	Processor::tracksMDL(targetTracks);	//轨迹提取特征点

	Processor::tracks2Segment(targetTracks, segments);//根据特征点生成子段
	double **disMat = Processor::disMatrice(segments);	//根据子段信息生成距离矩阵
	//printf("size:%d\n", segments.size());
	EpsiSolver epsiSolver = EpsiSolver((int)time(0), disMat,(int) segments.size());	//模拟退火求解聚类epsi
	epsiSolver.solve();
	double epsi = epsiSolver.getRes();
	//printf("DBSCAN epsi:%10g\n", epsi);
	DBSCAN res= DBSCAN(disMat, segments.size(), epsi, minPts);	//使用DBSCAN进行子段聚类
	for (int counter = 0; counter < segments.size(); counter++) {
		delete[] disMat[counter];
	}
	delete[] disMat;
	return res;
}

int Main::insertFreqRes(vector<vector<Track>> allFreqTracks,string Operator) {
	char* sql = new char[256];
	sprintf_s(sql, 256, "delete from m_selectedfrequentlytrack_main where operator = '%s';", Operator.data());

	SqlTool::operationExcutor(sql,SqlTool::res);
	int res = 0;
	for (vector<Track>tracks : allFreqTracks) {
		if (tracks.size() == 0)
			continue;
		for (Track t: tracks) {
			if (t.historyPoint.size() == 0)
				continue;
			SqlTool::insertExcutor(t.insertFreqSQL(Operator));
			+res;
			for (TrackPoint p : t.historyPoint) {
				SqlTool::insertExcutor(p.insertFreqSQL());
			}							
		}
	}
	mysql_commit(&SqlTool::mysql);
	return res;
}

void Main::trackSampling(vector<Track> tracks,int startLevel,int endLevel) {
	for (int counter = startLevel; counter <= endLevel; ++counter) {
		for (Track t : tracks) {
			SampledTrack s =TrackSimplify::simplify(t, counter, 2);			
			s.insertSampledDetail();
		}		
	}
}

void Main::miningInit() {
	SqlTool::operationExcutor("truncate m_historytrack_main", SqlTool::res);
	SqlTool::operationExcutor("truncate m_historytrack_sub", SqlTool::res);
	SqlTool::operationExcutor("truncate m_frequentlytrack_main", SqlTool::res);
	SqlTool::operationExcutor("truncate m_frequentlytrack_sub", SqlTool::res);
	SqlTool::operationExcutor("truncate m_sampling_historytrack_sub", SqlTool::res);
}

void Main::getTrackMain(vector<Track>& hisTracks, int argc, char* argv[]) {
	char* getMain = new char[256];
	sprintf_s(getMain, 256, "select TRACKID,POINTAMOUNT,TARGETID,STARTTIME,ENDTIME,LENGTH,SOURCE,TASKINFO,CONFIDENCELEVEL,\
OPERATOR,RESERVE1,RESERVE2 from m_selectedtrack_main where operator = '%s';", argv[argc - 1]);
	SqlTool::operationExcutor(getMain, SqlTool::res);
	//获取所有主轨迹信息
	while (SqlTool::column = mysql_fetch_row(SqlTool::res)) {
		hisTracks.push_back(Track(SqlTool::column[0], SqlTool::column[1], SqlTool::column[2],
			SqlTool::column[3], SqlTool::column[4], SqlTool::column[5], SqlTool::column[6],
			SqlTool::column[7], SqlTool::column[8], SqlTool::column[9], SqlTool::column[10], SqlTool::column[11]));
	}
	mysql_free_result(SqlTool::res);
}
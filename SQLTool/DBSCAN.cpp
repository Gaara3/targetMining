#include "stdafx.h"
#include "DBSCAN.h"
#include <fstream>
#include <iosfwd>
#include <math.h>

DBSCAN::~DBSCAN()
{
	/*for(int counter=0;counter<clus)*/
}

/*
函数：聚类初始化操作
说明：将距离矩阵半径，领域最小数据个数信息写入聚类算法类，读取文件，把数据信息读入写进算法类数据集合中
参数：
char* fileName;    //文件名
double radius;    //半径
int minPTs;        //领域最小数据个数
返回值： true;    */
 DBSCAN::DBSCAN(double** distMat, int size,double radius, int minPTs)	
{
	this->radius = radius;        //设置半径
	this->minPTs = minPTs;        //设置领域最小数据个数
	//this->dimNum = DIME_NUM;    //设置数据维度
	//this->distMat = distMat;
	this->dataNum = size;

	for (int counter = 0; counter < dataNum; counter++) {	//DataSets 初始化，置idx为counter,clusterID为-1，visited为false
		dataSets.push_back(DataPoint(counter));
	}

	for (int i = 0; i<dataNum; i++)
	{
		SetArrivalPoints(dataSets[i],i,distMat);            //计算数据点领域内对象
	}
	//return true;    //返回
}


/*
函数：设置数据点的领域点列表
说明：设置数据点的领域点列表
参数：
返回值： true;    */
void DBSCAN::SetArrivalPoints(DataPoint& dp,int idx,double** distMat)
{
	for (int i = 0; i<dataNum; i++)                //对每个数据点执行
	{
		//double distance = GetDistance(dataSets[i], dp);    //获取与特定点之间的距离
		if (distMat[idx][i] <= radius && i != dp.GetDpId())        //若距离小于半径，并且特定点的id与dp的id不同执行
			dp.GetArrivalPoints().push_back(i);            //将特定点id压入dp的领域列表中
	}
	/*if (dp.GetArrivalPoints().size() >= minPTs)            //若dp领域内数据点数据量> minPTs执行
	{
		dp.SetKey(true);    //将dp核心对象标志位设为true
		return;                //返回
	}
	dp.SetKey(false);    //若非核心对象，则将dp核心对象标志位设为false*/	//commented by lsx
	dp.SetKey(dp.GetArrivalPoints().size() >= minPTs);	//根据领域内的点数设置是否为核心点
}


/*
函数：执行聚类操作
说明：执行聚类操作
参数：
返回值： true;    */
int DBSCAN::DoDBSCANRecursive()
{
	int clusterId = 0;                        //聚类id计数，初始化为0
	for (int i = 0; i<dataNum; i++)            //对每一个数据点执行
	{
		DataPoint& dp = dataSets[i];                    //取到第i个数据点对象
		if (!dp.isVisited() && dp.IsKey())            //若对象没被访问过，并且是核心对象执行
		{
			dp.SetClusterId(clusterId);                //设置该对象所属簇ID为clusterId
			dp.SetVisited(true);                    //设置该对象已被访问过
			KeyPointCluster(i, clusterId);            //对该对象领域内点进行聚类
			clusterId++;                            //clusterId自增1
		}
		//cout << "孤立点\T" << i << endl;
	}

	//cout << "共聚类" << clusterId << "个" << endl;        //算法完成后，输出聚类个数
	return clusterId;    //返回
}

vector<int>* DBSCAN::clusterGenerate()
{
	this->clusterNum = this->DoDBSCANRecursive();
	vector<int>* clusterInfo = new vector<int>[clusterNum];
	for (int counter = 0; counter < dataNum; counter++) {
		int curCluster = dataSets[counter].GetClusterId();
		if (curCluster != -1)
			clusterInfo[curCluster].push_back(counter);
	}

	for (int counter = 0; counter < clusterNum; counter++) {
		//printf("cluster %d:",counter);
		for (vector<int>::iterator i = clusterInfo[counter].begin(); i != clusterInfo[counter].end(); i++) {
			//printf("%d\t", *i);
		}
		//printf("\n");
	}
		return clusterInfo;
}

/*
函数：对数据点领域内的点执行聚类操作
说明：采用递归的方法，深度优先聚类数据
参数：
int dpID;            //数据点id
int clusterId;    //数据点所属簇id
返回值： void;    */
void DBSCAN::KeyPointCluster(int dpID, int clusterId)
{
	DataPoint& srcDp = dataSets[dpID];        //获取数据点对象
	if (!srcDp.IsKey())    return;
	vector<int>& arrvalPoints = srcDp.GetArrivalPoints();        //获取对象领域内点ID列表
	for (int i = 0; i<arrvalPoints.size(); i++)
	{
		DataPoint& desDp = dataSets[arrvalPoints[i]];    //获取领域内点数据点
		if (!desDp.isVisited())                            //若该对象没有被访问过执行
		{
			//cout << "数据点\t"<< desDp.GetDpId()<<"聚类ID为\t" <<clusterId << endl;
			desDp.SetClusterId(clusterId);        //设置该对象所属簇的ID为clusterId，即将该对象吸入簇中
			desDp.SetVisited(true);                //设置该对象已被访问
			if (desDp.IsKey())                    //若该对象是核心对象
			{
				KeyPointCluster(desDp.GetDpId(), clusterId);    //递归地对该领域点数据的领域内的点执行聚类操作，采用深度优先方法
			}
		}
	}
}

//两数据点之间距离
/*
函数：获取两数据点之间距离
说明：获取两数据点之间的欧式距离
参数：
DataPoint& dp1;        //数据点1
DataPoint& dp2;        //数据点2
返回值： double;    //两点之间的距离        
double DBSCAN::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance = 0;        //初始化距离为0
	for (int i = 0; i<DIME_NUM; i++)    //对数据每一维数据执行
	{
		distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i], 2);    //距离+每一维差的平方
	}
	return pow(distance, 0.5);        //开方并返回距离
}*/
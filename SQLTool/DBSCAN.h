#pragma once
#include <iostream>
#include <cmath>
#include "DataPoint.h"

using namespace std;

//聚类分析类型
class DBSCAN
{
private:
	vector<DataPoint> dataSets;        //数据集合
	//  int dimNum;            //维度	直接传入距离矩阵，不再需要考虑数据维度   by lsx
	double radius;                    //半径
	int dataNum;            //数据数量
	int minPTs;            //邻域最小数据个数
	//double **distMat;				//距离矩阵
	

	//double GetDistance(DataPoint& dp1, DataPoint& dp2);                    //距离函数
	void SetArrivalPoints(DataPoint& dp,int idx,double ** distMat);                                //设置数据点的领域点列表
	void KeyPointCluster(int i, int clusterId);    //对数据点领域内的点执行聚类操作
public:
	int clusterNum;
	DBSCAN() {}                    //默认构造函数
	~DBSCAN();
	DBSCAN(double** distMat,int size, double radius, int minPTs);    //初始化操作
	int DoDBSCANRecursive();            //DBSCAN递归算法
	vector<int>* clusterGenerate();
};
#include "stdafx.h"
#include "DataPoint.h"

//默认构造函数
DataPoint::DataPoint()
{
}

DataPoint::DataPoint(int id)
{
	this->dpID = id;
	this->clusterId = -1;
	this->visited = false;
}

//构造函数
DataPoint::DataPoint(int dpID, double* dimension, bool isKey) :isKey(isKey), dpID(dpID)
{
	//传递每维的维度数据
	for (int i = 0; i<DIME_NUM; i++)
	{
		this->dimension[i] = dimension[i];
	}
}

//设置维度数据
void DataPoint::SetDimension(double* dimension)
{
	for (int i = 0; i<DIME_NUM; i++)
	{
		this->dimension[i] = dimension[i];
	}
}

//获取维度数据
double* DataPoint::GetDimension()
{
	return this->dimension;
}

//获取是否为核心对象
bool DataPoint::IsKey()
{
	return this->isKey;
}

//设置核心对象标志
void DataPoint::SetKey(bool isKey)
{
	this->isKey = isKey;
}

//获取DpId方法
int DataPoint::GetDpId()
{
	return this->dpID;
}

//设置DpId方法
void DataPoint::SetDpId(int dpID)
{
	this->dpID = dpID;
}

//GetIsVisited方法
bool DataPoint::isVisited()
{
	return this->visited;
}


//SetIsVisited方法
void DataPoint::SetVisited(bool visited)
{
	this->visited = visited;
}

//GetClusterId方法
int DataPoint::GetClusterId()
{
	return this->clusterId;
}

//GetClusterId方法
void DataPoint::SetClusterId(int clusterId)
{
	this->clusterId = clusterId;
}

//GetArrivalPoints方法
vector<int>& DataPoint::GetArrivalPoints()
{
	return arrivalPoints;
}
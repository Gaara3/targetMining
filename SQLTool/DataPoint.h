#pragma once
#include <vector>

using namespace std;

const int DIME_NUM=2;        //数据维度为2，全局常量

//数据点类型
class DataPoint
{
private:
    int dpID;                //数据点ID
    double dimension[DIME_NUM];        //维度数据
    int clusterId;                    //所属聚类ID
    bool isKey;                        //是否核心对象
    bool visited;                    //是否已访问
    vector<int> arrivalPoints;    //领域数据点id列表
public:
    DataPoint();                                                    //默认构造函数
	DataPoint(int id);
    DataPoint(int dpID,double* dimension , bool isKey);    //构造函数

    int GetDpId();                //GetDpId方法
    void SetDpId(int dpID);        //SetDpId方法
    double* GetDimension();                    //GetDimension方法
    void SetDimension(double* dimension);    //SetDimension方法
    bool IsKey();                            //GetIsKey方法
    void SetKey(bool isKey);                //SetKey方法
    bool isVisited();                        //GetIsVisited方法
    void SetVisited(bool visited);            //SetIsVisited方法
    int GetClusterId();                    //GetClusterId方法
    void SetClusterId(int classId);        //SetClusterId方法
    vector<int>& GetArrivalPoints();    //GetArrivalPoints方法
};

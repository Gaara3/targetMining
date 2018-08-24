#pragma once
#include <vector>

using std::vector;

template<class T>
struct Cluster
{
	char* clusterName;
	vector<T> sourceData;
	vector<int> *idx;
};



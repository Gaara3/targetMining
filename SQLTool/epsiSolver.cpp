#include "stdafx.h"
#include "EpsiSolver.h"
#include <stdlib.h>


EpsiSolver::EpsiSolver():SimulatedAnnualingSolver()
{
}

EpsiSolver::EpsiSolver(int rand_seed, double** distMat,int size) :SimulatedAnnualingSolver(rand_seed){
	this->distMat = distMat;
	this->size = size;
	double maxDis = 0;
	for (int counter1 = 0; counter1 < size; counter1++) {
		for (int counter2 = 0; counter2 < size; counter2++) {
			if (distMat[counter1][counter2] > maxDis)
				maxDis = distMat[counter1][counter2];
		}
	}
	this->maxDis = maxDis;
	res = 0.5*maxDis;
};

double EpsiSolver::getRes() { return this->res; }

double EpsiSolver::energy() {
	//得出每个点满足<epsi距离的出现次数
	//算出所有点的次数和
	//得出每个点对总次数的占比
	//-p*log(p)求和
	int *epsiCounter = new int[size];
	int totalSum = 0;
	for (int counter1 = 0; counter1 < size; counter1++) {
		epsiCounter[counter1] = 0;
		for (int counter2 = 0; counter2 < size; counter2++) {
			if (distMat[counter1][counter2] < this->res) {
				(epsiCounter[counter1])++;
				totalSum++;
				//printf("%d\t", epsiCounter[counter1]);
			}
		}
	}
	double res = 0;
	for (int counter = 0; counter < size; counter++) {
		//printf("%d\t", epsiCounter[counter]);
		double p = (double)epsiCounter[counter] / (double)totalSum;
		if (p == 0)
			continue;
		res += (-p)*log2l(p);
	}
	delete[]epsiCounter;
	return res;
}


//step_size为退火过程中求新解的步长，在求epsi中暂不使用步长求解，改为随机分配
//T为当前温度，此封装里为(0.01,1)用于获取next epsi值
double EpsiSolver::take_step(double step_size,double T) {	
	//res根据上一次的有效epsi赋予新值
	//计算新res对应的energy并返回
	double r = T * maxDis;
	double start = (epsi - r);
	double end = (epsi + r);
	if (start < 0)	start = 0;
	if (end > maxDis)	end = maxDis;
	std::uniform_real_distribution<> dis(start, end);
	res = dis(m_randGenerator);
	return energy();
}

void EpsiSolver::undo_step() {//epsi为上一次的出发解
	res = epsi;
}

void EpsiSolver::print() {//已将基类debug print打开，此处先不打印计算过程
	//printf("current res:%12g", res);
}

void EpsiSolver::save_best()//若接受该解，则更新epsi值
{
	this->epsi = this->res;
}



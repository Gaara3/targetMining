#pragma once

#include "SimulatedAnnualingSolver.h"
class EpsiSolver :
	public SimulatedAnnualingSolver
{
private:
	double** distMat;
	double epsi;
	double res;
	int size;
	double maxDis;
public:
	EpsiSolver();
	EpsiSolver(int rand_seed,double** distMat,int size);
	double energy() override;
	double take_step(double step_size,double T) override;
	void undo_step()override;
	void print()override;
	void save_best()override;
	double getRes();
};

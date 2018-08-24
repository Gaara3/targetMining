#pragma once
#ifndef SIMULATEDANNUALINGSOLVER_H
#define SIMULATEDANNUALINGSOLVER_H

#include<random>

class SimulatedAnnualingSolver
{
public:
	SimulatedAnnualingSolver();
	SimulatedAnnualingSolver(int rand_seed);
	void setParameters(double t_initial, double mu_t, double k = 1, double t_min = 1e-5, int iters_fixed_T = 100, double step_size = 1);
	void enablePrint(bool on) { m_print_position = on; }
	void solve();
protected:
	virtual double take_step(double step_size,double T) = 0;
	virtual void undo_step() = 0;
	virtual void save_best() = 0;
	virtual double energy() = 0;
	virtual void print() = 0;
	std::mt19937 m_randGenerator;
private:
	bool m_print_position;
	int m_iters_fixed_T;
	double m_step_size;
	double m_k;
	double m_t_initial;
	double m_mu_t;
	double m_t_min;
	double boltzmann(double E, double new_E, double T, double k);
	bool accept(double new_E, double E,double T);
};

#endif // SIMULATEDANNUALINGSOLVER_H
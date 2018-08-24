#include "stdafx.h"
#include "SimulatedAnnualingSolver.h"
#include <cmath>
#include <iostream>
#define GSL_LOG_DBL_MIN   (-7.0839641853226408e+02)

SimulatedAnnualingSolver::SimulatedAnnualingSolver()
{
}

SimulatedAnnualingSolver::SimulatedAnnualingSolver(int rand_seed)
	:m_t_initial(1),
	m_mu_t(1.01),
	m_k(1),
	m_t_min(0.01),
	m_iters_fixed_T(100),
	m_print_position(false),	//为debug改为true
	m_step_size(1),
	m_randGenerator(rand_seed)
{

}

void SimulatedAnnualingSolver::setParameters(double t_initial, double mu_t, double k, double t_min, int iters_fixed_T, double step_size)
{
	m_t_initial = t_initial;
	m_mu_t = mu_t;
	m_k = k;
	m_t_min = t_min;
	m_iters_fixed_T = iters_fixed_T;
	m_step_size = step_size;
}

void SimulatedAnnualingSolver::solve()
{
	int n_evals = 1, n_iter = 0;
	double E = energy();
	double best_E = E;
	save_best(); // 将当前状态存为最佳解

	double T = m_t_initial;
	double T_factor = 1.0 / m_mu_t;
	std::uniform_real_distribution<> dis(0, 1);
	while (T > m_t_min)	//外循环，模拟退火过程
	{
		int n_accepts = 0;
		int n_rejects = 0;
		int n_eless = 0;

		for (int i = 0; i < m_iters_fixed_T; ++i)//内循环，求得在一定温度下的最优解
		{
			double new_E = take_step(m_step_size,T);
			/*if (new_E <= best_E)
			{
				best_E = new_E;
				save_best();
			}
*/  //seems duplicated   commented by lsx
			++n_evals;
			if (new_E < E)
			{			
				best_E = new_E;
				save_best();				
				/* yay! take a step */
				E = new_E;
				++n_eless;
			}
			else if (dis(m_randGenerator) < boltzmann(E, new_E, T, m_k))
			{
				/* yay! take a step */
				E = new_E;
				++n_accepts;
				save_best();
			}
			else
			{
				undo_step(); // 回退到上一个状态
				++n_rejects;
			}
		}

		if (m_print_position)
		{
			//printf("%5d   %7d  %12g", n_iter, n_evals, T);
			printf("T: %10g,n_accept:%d", T,n_accepts);
			print();
			printf("Energy:%12g\n", E);
			//printf("  %12g  %12g\n", E, best_E);
		}
		/* apply the cooling schedule to the temperature */
		T *= T_factor;
		++n_iter;		
	}
}

inline double SimulatedAnnualingSolver::boltzmann(double E, double new_E, double T, double k)
{
	double x = -(new_E - E) / (k * T);
	/* avoid underflow errors for large uphill steps */
	return (x < GSL_LOG_DBL_MIN) ? 0.0 : exp(x);
}

bool SimulatedAnnualingSolver::accept(double new_E, double E,double T) {
	return (new_E < E) || (double(rand())/double(RAND_MAX) < boltzmann(E, new_E, T, m_k));
}
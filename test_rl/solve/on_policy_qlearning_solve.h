#pragma once
#include "temporal_difference_solve.h"

class on_policy_qlearning_solve : public temporal_difference_solve
{
public:
	on_policy_qlearning_solve(double gama, double error, solve_notify* notify);
	virtual void runing();
};
using on_policy_qlearning_solve_ptr = std::shared_ptr<on_policy_qlearning_solve>;

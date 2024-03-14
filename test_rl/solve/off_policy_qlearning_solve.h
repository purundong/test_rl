#pragma once
#include "temporal_difference_solve.h"
class off_policy_qlearning_solve: public temporal_difference_solve
{
public:
	off_policy_qlearning_solve(double gama, double error, solve_notify* notify);
	void update_target_pi(std::unordered_map<action_ptr, double>& a_target_pi);
public:
	virtual void runing();
};


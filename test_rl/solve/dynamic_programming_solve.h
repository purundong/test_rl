#pragma once
#include "abstract_solve.h"

class dynamic_programming_solve :public abstract_solve
{
	int _evaluate_bout{ 50 };
public:
	dynamic_programming_solve(double gama, double error, solve_notify* notify);
	virtual void runing();
	virtual QWidget* make_widget();
	virtual QWidget* make_state_widget(state_ptr obj);
public:
	void policy_evaluate();
	bool policy_improve();
	int evaluate_bout();
	void set_evaluate_bout(int evaluate_bout);
};
using dynamic_programming_ptr = std::shared_ptr<dynamic_programming_solve>;


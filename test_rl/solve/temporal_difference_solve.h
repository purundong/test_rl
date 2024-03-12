#pragma once
#include "monte_carlo_solve.h"
#include <unordered_map>
class ui_td_state;

class temporal_difference_solve : public monte_carlo_solve
{
private:
	std::unordered_map<state_ptr, ui_td_state*> _map_td_state;
protected:
	double _lamda{ 0.005 };
public:
	temporal_difference_solve(double gama, double error, solve_notify* notify);
public:
	virtual QWidget* make_widget();
	virtual QWidget* make_state_widget(state_ptr obj);
public:
	virtual void runing();
	double lamda();
	void set_lamda(double lamda);
};
using temporal_difference_solve_ptr = std::shared_ptr<temporal_difference_solve>;
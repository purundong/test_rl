#pragma once
#include "abstract_solve.h"

class monte_carlo_solve : public abstract_solve
{
protected:
	int _episode_num{ 1 }, _episode_size{ 100 };
	bool _episode_show{ false };
	double _epsilon{ 0.5 };
public:
	monte_carlo_solve(double gama, double error, solve_notify* notify);
	virtual void runing();
	virtual QWidget* make_widget();
	virtual QWidget* make_state_widget(state_ptr obj);
public:
	int episode_size();
	void set_episode_size(int episode_size);
	int episode_num();
	void set_episode_num(int episode_num);
	bool episode_show();
	void set_episode_show(bool episode_show);
	double epsilon();
	void set_epsilon(double epsilon);
};
using monte_carlo_ptr = std::shared_ptr<monte_carlo_solve>;
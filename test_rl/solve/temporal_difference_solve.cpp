#include "temporal_difference_solve.h"
#include "state/state.h"
#include <unordered_map>
#include <boost/thread.hpp>
#include "ui/ui_td_solve.h"
#include "ui/ui_td_state.h"

temporal_difference_solve::temporal_difference_solve(double gama, double error, solve_notify* notify) :
	monte_carlo_solve(gama, error, notify)
{
}

QWidget* temporal_difference_solve::make_widget()
{
	return new ui_td_solve(shared_from_this());
}

QWidget* temporal_difference_solve::make_state_widget(state_ptr obj)
{
	auto* widget = new ui_td_state(obj);
	_map_td_state[obj] = widget;
	return widget;
}

void temporal_difference_solve::runing()
{
	_notify->on_solve_start();
	for (int i = 0; i < _episode_num; ++i)
	{
		auto curr_state = (*(**_states).begin()).second; //t时刻的状态
		auto curr_a = curr_state->sample_action(); //t时刻的动作

		for (int j = 0; j < _episode_size; ++j)
		{
			boost::this_thread::interruption_point();
			auto next_state = curr_a->sample_state(); //t+1时刻的状态
			auto next_a = next_state->sample_action(); //t+1时刻的动作
			auto td_target = (next_state->sample_reword() + _gama * next_a->value());
			auto td_error = curr_a->value() - td_target;
			auto a_value = curr_a->value() - _lamda * (td_error);
			_map_td_state[curr_state]->set_error(curr_a->type(), td_error);
			curr_a->set_value(a_value);
			curr_state->update_value();
			curr_state->update_policy_for_epsilon_greedy(_epsilon);
			curr_state = next_state; //更新状态
			curr_a = next_a;//更新动作
			if (_episode_show)
				_notify->on_state_change(curr_state, curr_a->type());
		}
	}
	_notify->on_solve_end();
}

double temporal_difference_solve::lamda()
{
	return _lamda;
}

void temporal_difference_solve::set_lamda(double lamda)
{
	_lamda = lamda;
}

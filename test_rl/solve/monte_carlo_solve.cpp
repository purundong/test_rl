#include "monte_carlo_solve.h"
#include "state/state.h"
#include <unordered_map>
#include "ui/ui_monte_carlo.h"
#include <boost/thread.hpp>
#include "ui/ui_monte_carlo_state.h"

monte_carlo_solve::monte_carlo_solve(double gama, double error, solve_notify* notify) :
	abstract_solve(gama, error, notify)
{
}

void monte_carlo_solve::runing()
{
	_notify->on_solve_start();
	std::unordered_map<std::string, std::unordered_map<action::action_type, double>> q_vals; //状态动作价值表
	std::unordered_map<std::string, std::unordered_map<action::action_type, std::list<double>>> q_vals_list; //状态动作的回报列表
	std::list<std::tuple<std::string, action::action_type, double>> episode; //轨迹链表
	for (int i = 0; i < _episode_num; ++i)
	{
		auto state_obj = (*(**_states).begin()).second;

		for (int j = 0; j < _episode_size; ++j) //执行一条轨迹
		{
			boost::this_thread::interruption_point();
			auto& s_name = state_obj->get_name();
			auto a = state_obj->sample_action();
			auto a_type = a->type();
			state_obj = a->sample_state();
			auto reword = state_obj->sample_reword();
			episode.push_back(std::make_tuple(s_name, a_type, reword));
			if (_episode_show)
				_notify->on_state_change(state_obj, a_type);
		}

		auto rbegin = episode.rbegin();
		q_vals_list[std::get<0>(*rbegin)][std::get<1>(*rbegin)].push_front(std::get<2>(*rbegin));

		for (auto previous = rbegin++; rbegin != episode.rend(); ++previous, ++rbegin) { //轨迹转换每个状态动作的回报
			auto& q_val_list = q_vals_list[std::get<0>(*rbegin)][std::get<1>(*rbegin)];
			auto p = *q_vals_list[std::get<0>(*previous)][std::get<1>(*previous)].begin();
			q_val_list.push_front(std::get<2>(*rbegin) + (_gama * p));
		}

		for (auto& [name, a_map] : q_vals_list) { //计算每个状态动作的平均回报作为状态动作价值
			for (auto& [a_type, q_val_list] : a_map) {
				double all_val{ 0.0 };
				for (auto& val : q_val_list)
					all_val += val;
				q_vals[name][a_type] = all_val / q_val_list.size();
			}
		}

		for (auto& [name, a_map] : q_vals) // 更新状态动作价值表
		{
			auto state = (**_states)[name];
			for (auto& [a_type, val] : a_map)
				state->set_action_value(a_type, val);
			state->update_value(); //更新状态价值
		}

		for (auto& [name, state_obj] : (**_states))
			state_obj->update_policy_for_epsilon_greedy(_epsilon);
		episode.clear();
	}
	_notify->on_solve_end();
}

QWidget* monte_carlo_solve::make_widget()
{
	return new ui_monte_carlo(shared_from_this());
}

QWidget* monte_carlo_solve::make_state_widget(state_ptr obj)
{
	return new ui_monte_carlo_state(obj);
}

int monte_carlo_solve::episode_size()
{
	return _episode_size;
}

void monte_carlo_solve::set_episode_size(int episode_size)
{
	_episode_size = episode_size;
}

int monte_carlo_solve::episode_num()
{
	return _episode_num;
}

void monte_carlo_solve::set_episode_num(int episode_num)
{
	_episode_num = _episode_num;
}

bool monte_carlo_solve::episode_show()
{
	return _episode_show;
}

void monte_carlo_solve::set_episode_show(bool episode_show)
{
	_episode_show = episode_show;
}

double monte_carlo_solve::epsilon()
{
	return _epsilon;
}

void monte_carlo_solve::set_epsilon(double epsilon)
{
	_epsilon = epsilon;
}
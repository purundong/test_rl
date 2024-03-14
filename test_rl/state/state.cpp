#include "state.h"
#include <algorithm>
#include "rand_num.h"
#include <unordered_set>
#include <iostream>
#include "ui/ui_reword.h"
#include "ui/ui_state_transfers.h"
#include "ui/ui_state_info.h"

map_state::map_state(int row_size, int col_size, int trap, int target)
{
	for (int r = 0; r < row_size; ++r)
		for (int c = 0; c < col_size; ++c) {
			auto s = std::make_shared<state>(r, c);
			s->set_reward(0.0, 1.0);
			_map_state.emplace(s->get_name(), s);
		}

	auto factory = std::make_unique<map_action_factory>();
	for (int r = 1; r < row_size - 1; ++r)
		for (int c = 1; c < col_size - 1; ++c) {
			if (auto key = _map_state.find(state::get_name(r, c)); key != _map_state.end())
				key->second->set_map_action(factory->create_map_action(_map_state, r, c));
		}

	factory = std::make_unique<map_action_top_factory>();
	for (int c = 1; c < col_size - 1; ++c)
	{	//第一行
		if (auto key = _map_state.find(state::get_name(0, c)); key != _map_state.end())
			key->second->set_map_action(factory->create_map_action(_map_state, 0, c));
	}

	factory = std::make_unique<map_action_bot_factory>();
	for (int c = 1; c < col_size - 1; ++c)
	{	//最后一行
		if (auto key = _map_state.find(state::get_name(row_size - 1, c)); key != _map_state.end())
			key->second->set_map_action(factory->create_map_action(_map_state, row_size - 1, c));
	}

	factory = std::make_unique<map_action_left_factory>();
	for (int r = 1; r < row_size - 1; ++r)
	{//第一列
		if (auto key = _map_state.find(state::get_name(r, 0)); key != _map_state.end())
			key->second->set_map_action(factory->create_map_action(_map_state, r, 0));
	}

	factory = std::make_unique<map_action_right_factory>();
	for (int r = 1; r < row_size - 1; ++r)
	{//最后一列
		if (auto key = _map_state.find(state::get_name(r, col_size - 1)); key != _map_state.end())
			key->second->set_map_action(factory->create_map_action(_map_state, r, col_size - 1));
	}

	factory = std::make_unique<map_action_upper_left_factory>();

	if (auto key = _map_state.find(state::get_name(0, 0)); key != _map_state.end())
		key->second->set_map_action(factory->create_map_action(_map_state, 0, 0));

	factory = std::make_unique<map_action_lower_left_factory>();

	if (auto key = _map_state.find(state::get_name(row_size - 1, 0)); key != _map_state.end())
		key->second->set_map_action(factory->create_map_action(_map_state, row_size - 1, 0));

	factory = std::make_unique<map_action_upper_right_factory>();

	if (auto key = _map_state.find(state::get_name(0, col_size - 1)); key != _map_state.end())
		key->second->set_map_action(factory->create_map_action(_map_state, 0, col_size - 1));

	factory = std::make_unique<map_action_lower_right_factory>();

	if (auto key = _map_state.find(state::get_name(row_size - 1, col_size - 1)); key != _map_state.end())
		key->second->set_map_action(factory->create_map_action(_map_state, row_size - 1, col_size - 1));

	rand_num rand;
	std::unordered_set<std::string> num_set;
	for (int i = 0; i < trap; ++i) {
		while (true)
		{
			int row = rand.get_rand(row_size);
			int col = rand.get_rand(col_size);
			auto name = state::get_name(row, col);
			if (num_set.find(name) == num_set.end())
			{
				if (auto key = _map_state.find(name); key != _map_state.end())
					key->second->reset_reward(-1, 1);
				num_set.emplace(name);
				break;
			}
		}
	}

	for (int i = 0; i < target; ++i) {
		while (true)
		{
			int row = rand.get_rand(row_size);
			int col = rand.get_rand(col_size);
			auto name = state::get_name(row, col);
			if (num_set.find(name) == num_set.end())
			{
				if (auto key = _map_state.find(name); key != _map_state.end())
					key->second->reset_reward(1, 1);
				num_set.emplace(name);
				break;
			}
		}
	}
}

state::state(int x, int y) :_x(x), _y(y), _name{ get_name(_x, _y) }, _value{ 0.0 },
_policy({ {action::action_type::up, 0.25},
			{action::action_type::right, 0.25},
			{action::action_type::down, 0.25},
			{action::action_type::left, 0.25},
			{action::action_type::fixed, 0.25} })
{
}

double state::reward_expectations()
{
	double expectations = 0.0;
	std::for_each(_rewards.begin(), _rewards.end(), [&expectations](auto& p) {expectations += p.first * p.second; });
	return expectations;
}

action_ptr state::sample_action()
{
	std::vector<double> vec(_policy.size());
	auto fun = [](const std::pair<const action::action_type, double>& pa) { return pa.second; };
	std::transform(_policy.begin(), _policy.end(), vec.begin(), fun);
	auto a = (action::action_type)rand_num::sampling(vec);
	return (*_map_action)[a];
}

reward state::sample_reword()
{
	std::vector<double> vec(_rewards.size());
	std::vector<double> rewards;
	rewards.reserve(_rewards.size());
	auto fun = [&rewards](const std::pair<const reward, probability>& pa) {
		rewards.push_back(pa.first);
		return pa.second;
		};
	std::transform(_rewards.begin(), _rewards.end(), vec.begin(), fun);
	auto a = rand_num::sampling(vec);
	return rewards[a];
}

action_ptr state::max_action()
{
	auto fun = [](const auto& a, const auto& b) {return a.second->value() < b.second->value(); };
	auto max = std::max_element(_map_action->begin(), _map_action->end(), fun);
	return max->second;
}

QColor state::get_color()
{
	auto rewrod = reward_expectations();
	if (rewrod == 0)
		return QColor(255, 255, 255);
	else if (rewrod > 0)
		return QColor(51, 51, 255);
	else
		return QColor(153, 0, 76);
}

std::shared_ptr<QDialog> state::make_set_reword()
{
	return std::dynamic_pointer_cast<QDialog>(std::make_shared<ui_reword>(shared_from_this()));
}

std::shared_ptr<QDialog> state::make_set_state_transfers(map_state_ptr states)
{
	return std::dynamic_pointer_cast<QDialog>(std::make_shared<ui_state_transfers>(shared_from_this(), states));
}

std::shared_ptr<QDialog> state::make_show_state_info()
{
	return std::dynamic_pointer_cast<QDialog>(std::make_shared<ui_state_info>(shared_from_this()));
}

std::vector<expect> state::get_state_transfer(map_state_ptr states)
{
	std::vector<expect> vec_prob;
	vec_prob.reserve((**states).size());
	for (int i = 0; auto & [name, state] : **states)
	{
		double prob = 0.0;
		for (auto& [action_type, action_obj] : *_map_action)
			prob += _policy[action_type] * action_obj->get_state_transfer(name);
		vec_prob.push_back(prob);
	}
	return vec_prob;
}

expect state::get_state_reward()
{
	expect value = 0.0;
	for (auto& [action_type, action_obj] : *_map_action)
		value += _policy[action_type] * action_obj->get_reward_expectations();
	return value;
}

std::vector<double> state::get_action_value()
{
	std::vector<double> values;
	values.reserve(_map_action->size());
	for (auto& [action_type, action_obj] : *_map_action)
		values.push_back(action_obj->value());
	return values;
}

void state::update_action_value(double gama)
{
	for (auto& [action_type, action_obj] : *_map_action)
		action_obj->update_value(gama);
}

std::map<action::action_type, probability>& state::policy()
{
	return _policy;
}

std::vector<double> state::get_policy()
{
	std::vector<double> vec_policy;
	vec_policy.reserve(_policy.size());
	for (auto& [action_type, prob] : _policy)
		vec_policy.push_back(prob);
	return vec_policy;
}

void state::update_value()
{
	_value = 0.0;
	for (auto& [action_type, action_obj] : *_map_action) {
		_value += _policy[action_type] * action_obj->value();
	}

}

void state::update_policy_for_greedy()
{
	auto max = std::max_element(_map_action->begin(), _map_action->end(), [](const auto& a, const auto& b) {return a.second->value() < b.second->value(); });
	for (auto& [action_type, action_obj] : *_map_action)
		_policy[action_type] = (action_type == max->first) ? 1.0 : 0.0;
}

void state::update_policy_for_epsilon_greedy(double epsilon)
{
	auto action_size = _map_action->size();
	auto max = std::max_element(_map_action->begin(), _map_action->end(), [](const auto& a, const auto& b) {return a.second->value() < b.second->value(); });
	for (auto& [action_type, action_obj] : *_map_action)
		_policy[action_type] = (action_type == max->first) ? 1 - (epsilon / action_size * (action_size - 1)) : epsilon / action_size;
}

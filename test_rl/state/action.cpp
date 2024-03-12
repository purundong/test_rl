#include "action.h"
#include <vector>
#include <algorithm>
#include "rand_num.h"
#include "state.h"

const int action::action_size = 5;

std::string action::get_name(action_type type)
{
	switch (type)
	{
	case up: { return "上"; }break;
	case right: { return "右"; }break;
	case down: { return "下"; }break;
	case left: { return "左"; }break;
	case fixed: { return "原"; }break;
	default: {return "错误"; }break;
	}
}

action::action(const action_type type) :_type(type), _name(get_name(type)), _value{ 0.0 }
{
}

void action::set_state_transfer(state_ptr s, probability p)
{
	_state_transfer[s->get_name()] = std::make_tuple(s, p);
}

void action::del_state_transfer(const std::string& name)
{
	_state_transfer.erase(name);
}

expect action::get_state_transfer(const std::string& name)
{
	if (auto key = _state_transfer.find(name); key != _state_transfer.end())
		return std::get<1>(key->second);
	else
		return 0.0;
}

expect action::get_reward_expectations()
{
	expect val{ 0.0 };
	for (auto& [name, s] : _state_transfer)
		val += std::get<0>(s).lock()->reward_expectations() * std::get<1>(s);
	return val;
}

void action::update_value(const double& gama)
{
	auto value = 0.0;
	for (auto& [name, s] : _state_transfer) {
		auto state_obj = std::get<0>(s).lock();
		value += state_obj->reward_expectations() + gama * state_obj->value();
	}
	_value = value;
}

state_ptr action::sample_state()
{
	std::vector<double> vec(_state_transfer.size());
	std::vector<state_ptr> states;
	states.reserve(_state_transfer.size());
	auto fun = [&states](const std::pair<const std::string, std::tuple<std::weak_ptr<state>, probability>>& pa) {
		states.push_back(std::get<0>(pa.second).lock());
		return std::get<1>(pa.second); 
		};
	std::transform(_state_transfer.begin(), _state_transfer.end(), vec.begin(), fun);
	auto a = rand_num::sampling(vec);
	return states[a];
}

map_action_ptr map_action_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = std::make_shared<map_action>();
	(*map_action_obj)[action::action_type::up] = std::make_shared<action>(action::action_type::up);
	auto name = state::get_name(x - 1, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::up]->set_state_transfer(s_key->second, 1.0);

	(*map_action_obj)[action::action_type::right] = std::make_shared<action>(action::action_type::right);
	name = state::get_name(x, y + 1);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::right]->set_state_transfer(s_key->second, 1.0);

	(*map_action_obj)[action::action_type::down] = std::make_shared<action>(action::action_type::down);
	name = state::get_name(x + 1, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::down]->set_state_transfer(s_key->second, 1.0);

	(*map_action_obj)[action::action_type::left] = std::make_shared<action>(action::action_type::left);
	name = state::get_name(x, y - 1);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::left]->set_state_transfer(s_key->second, 1.0);

	(*map_action_obj)[action::action_type::fixed] = std::make_shared<action>(action::action_type::fixed);
	name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::fixed]->set_state_transfer(s_key->second, 1.0);

	return map_action_obj;
}

map_action_ptr map_action_top_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::up]->set_state_transfer(s_key->second, 1.0);
	return map_action_obj;
}

map_action_ptr map_action_bot_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::down]->set_state_transfer(s_key->second, 1.0);
	return map_action_obj;
}

map_action_ptr map_action_left_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::left]->set_state_transfer(s_key->second, 1.0);
	return map_action_obj;
}

map_action_ptr map_action_right_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end())
		(*map_action_obj)[action::action_type::right]->set_state_transfer(s_key->second, 1.0);
	return map_action_obj;
}

map_action_ptr map_action_upper_left_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end()) {
		(*map_action_obj)[action::action_type::up]->set_state_transfer(s_key->second, 1.0);
		(*map_action_obj)[action::action_type::left]->set_state_transfer(s_key->second, 1.0);
	}
	return map_action_obj;
}

map_action_ptr map_action_lower_left_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end()) {
		(*map_action_obj)[action::action_type::down]->set_state_transfer(s_key->second, 1.0);
		(*map_action_obj)[action::action_type::left]->set_state_transfer(s_key->second, 1.0);
	}
	return map_action_obj;
}

map_action_ptr map_action_upper_right_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end()) {
		(*map_action_obj)[action::action_type::up]->set_state_transfer(s_key->second, 1.0);
		(*map_action_obj)[action::action_type::right]->set_state_transfer(s_key->second, 1.0);
	}
	return map_action_obj;
}

map_action_ptr map_action_lower_right_factory::create_map_action(std::map<std::string, state_ptr>& states, int x, int y)
{
	auto map_action_obj = map_action_factory::create_map_action(states, x, y);
	auto name = state::get_name(x, y);
	if (auto s_key = states.find(name); s_key != states.end()) {
		(*map_action_obj)[action::action_type::down]->set_state_transfer(s_key->second, 1.0);
		(*map_action_obj)[action::action_type::right]->set_state_transfer(s_key->second, 1.0);
	}
	return map_action_obj;
}

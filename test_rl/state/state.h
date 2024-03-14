#pragma once
#include <map>
#include <vector>
#include <format>
#include "action.h"
#include <QColor>
#include <QDialog>

using probability = double;
using expect = double;
using reward = double;
using transfer = std::string;

class state;
using state_ptr = std::shared_ptr<state>;

class map_state;
using map_state_ptr = std::shared_ptr<map_state>;

class state : public std::enable_shared_from_this<state>
{
private:
	const int _x, _y;
	const std::string _name;
	double _value;//当前状态的价值
private:
	std::map<reward, probability> _rewards;//当前状态的奖励与获得概率(从其他状态转移到本状态)
	map_action_ptr _map_action; //当前状态的动作
	std::map<action::action_type, probability> _policy; //当前状态的动作概率(策略)
public:
	state(int x, int y);
public:
	inline const int& x() { return _x; }
	inline const int& y() { return _y; }
	inline double value() { return _value; };
	inline const std::string& get_name() {return _name;}
	inline void set_reward(reward r, probability p) { _rewards[r] = p; }
	inline void reset_reward(reward r, probability p) { _rewards.clear(); _rewards[r] = p; }
	inline void del_reward(reward r) { _rewards.erase(r); }
	inline void set_value(double v) { _value = v; }
	inline void set_map_action(map_action_ptr actions) { _map_action = actions; }
	inline probability policy(const action::action_type& type) { return _policy[type]; }
	inline std::map<reward, probability>& rewards() { return _rewards; }
	inline map_action_ptr map_action() { return _map_action; }
	inline void set_action_value(const action::action_type& type, double value) { (*_map_action)[type]->set_value(value); }
public:
	double reward_expectations(); //获取当前状态的奖励期望
	std::vector<expect> get_state_transfer(map_state_ptr states); //获取当前状态转移到其他所有状态的概率(向量)
	expect get_state_reward(); //获取当前状态在做出动作之后的奖励期望(标量)
public:
	std::vector<double> get_action_value(); //获取当前状态的动作价值(向量)
	void update_action_value(double gama); //更新当前状态的动作价值
	std::map<action::action_type, probability>& policy(); //获取当前状态的策略
	std::vector<double> get_policy(); //获取当前状态的策略
	void update_value(); //更新当前状态的价值
public:
	void update_policy_for_greedy(); //更新当前状态的策略
	void update_policy_for_epsilon_greedy(double epsilon); //更新当前状态的策略
public:
	action_ptr sample_action(); //根据策略抽样一个动作
	reward sample_reword(); //采样一个奖励
	action_ptr max_action(); //获取当前状态的最优动作
public:
	QColor get_color(); //获取当前状态的颜色
	std::shared_ptr<QDialog> make_set_reword(); //获取当前状态的控件
	std::shared_ptr<QDialog> make_set_state_transfers(map_state_ptr states); //获取当前状态的控件
	std::shared_ptr<QDialog> make_show_state_info(); //获取当前状态的控件
public:
	static std::string get_name(int x, int y) { return std::format("{:0>3}_{:0>3}", x, y); };
};

class map_state
{
	std::map<std::string, state_ptr> _map_state;
public:
	map_state(int row_size, int col_size, int trap, int target); //行数，列数，陷阱数，目标数
	std::map<std::string, state_ptr>& operator* () { return _map_state; }
	std::map<std::string, state_ptr>& operator-> () { return _map_state; }
};


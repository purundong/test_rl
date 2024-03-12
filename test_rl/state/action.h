#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <tuple>

using expect = double;

class state;
using state_ptr = std::shared_ptr<state>;

class map_state;
using map_state_ptr = std::shared_ptr<map_state>;

using probability = double;

class action
{
public:
	enum action_type : int { up = 0, right = 1, down = 2, left = 3, fixed = 4 };
	static const int action_size;
public:
	static std::string get_name(action_type type);
private:
	const action_type _type; //动作的类型
	const std::string _name; //动作的名字
	double _value; //动作的价值
	std::map<std::string, std::tuple<std::weak_ptr<state>, probability>> _state_transfer; //做出这个动作后的转移的状态和对应的概率
public:
	inline double value() { return _value; };
	inline void set_value(const double& value) { _value = value; }
	inline const std::string& get_name() { return _name; }
	inline std::map<std::string, std::tuple<std::weak_ptr<state>, probability>>& state_transfer(){return _state_transfer; }
	inline action_type type() { return _type; }
public:
	action(const action_type type);
	void set_state_transfer(state_ptr s, probability p);
	void del_state_transfer(const std::string& name);
public:
	expect get_state_transfer(const std::string& name); //获取转移到name状态的概率
	expect get_reward_expectations(); //做出动作后所获得的及时奖励的期望
	void update_value(const double& gama);
	state_ptr sample_state(); //根据动作转移概率抽样一个状态
};

using action_ptr = std::shared_ptr<action>;
using map_action = std::map<action::action_type, action_ptr>;
using map_action_ptr = std::shared_ptr<map_action>;

class map_action_factory 
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_top_factory :public map_action_factory //上边
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_bot_factory :public map_action_factory //下边
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_left_factory :public map_action_factory //左边
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_right_factory :public map_action_factory //右边
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_upper_left_factory :public map_action_factory //左上角
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_lower_left_factory :public map_action_factory //左下角
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_upper_right_factory :public map_action_factory //右上角
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};

class map_action_lower_right_factory :public map_action_factory //右下角
{
public:
	virtual map_action_ptr create_map_action(std::map<std::string, state_ptr>& states, int x, int y);
};





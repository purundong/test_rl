#include "dynamic_programming_solve.h"
#include <Eigen/Dense>
#include "ui/ui_dynamic_programming.h"
#include <boost/thread.hpp>

dynamic_programming_solve::dynamic_programming_solve(double gama, double error, solve_notify* notify) :
	abstract_solve(gama, error, notify)
{
}

void dynamic_programming_solve::runing()
{
	_notify->on_solve_start();
	while (true)
	{
		boost::this_thread::interruption_point();
		policy_evaluate();
		if (policy_improve())
			break;
	}
	_notify->on_solve_end();
}

void dynamic_programming_solve::policy_evaluate()
{
	Eigen::MatrixXd r_pi((**_states).size(), 1), p_pi((**_states).size(), (**_states).size());
	Eigen::MatrixXd v_pi = Eigen::MatrixXd::Zero((**_states).size(), 1);

	for (int r = 0; auto & [name, s] : **_states) {
		r_pi(r, 0) = s->get_state_reward();
		v_pi(r, 0) = s->value();
		auto p_ver = s->get_state_transfer(_states);
		for (int c = 0; c < p_ver.size(); ++c)
			p_pi(r, c) = p_ver[c];
		++r;
	}

	for (int i = 0; i < _evaluate_bout; ++i)
		v_pi = r_pi + (_gama * p_pi * v_pi);

	for (int r = 0; auto & [name, s] : **_states) {
		s->set_value(v_pi(r, 0));
		s->update_action_value(_gama);
		++r;
	}
}

bool dynamic_programming_solve::policy_improve()
{
	Eigen::MatrixXd old_policy((**_states).size(), action::action_size), new_policy((**_states).size(), action::action_size); //5个动作
	for (int r = 0; auto & [name, s] : **_states)
	{
		auto policy = s->get_policy();
		for (int c = 0; c < policy.size(); ++c)
			old_policy(r, c) = policy[c];
		s->update_policy_for_greedy();
		policy = s->get_policy();
		for (int c = 0; c < policy.size(); ++c)
			new_policy(r, c) = policy[c];
		++r;
	}
	auto diff = new_policy - old_policy;
	return diff.isZero(_error); //如果策略不在变化，返回true 结束求解 (当然终止迭代的办法还有其他的)
}

int dynamic_programming_solve::evaluate_bout()
{
	return _evaluate_bout;
}

void dynamic_programming_solve::set_evaluate_bout(int evaluate_bout)
{
	_evaluate_bout = evaluate_bout;
}

QWidget* dynamic_programming_solve::make_widget()
{
	return new ui_dynamic_programming(shared_from_this());
}

QWidget* dynamic_programming_solve::make_state_widget(state_ptr obj)
{
	return nullptr;
}
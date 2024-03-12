#pragma once
#include "state/state.h"
#include <QWidget>

class solve_notify
{
public:
	virtual void on_solve_start() = 0;
	virtual void on_solve_end() = 0;
	virtual void on_state_change(state_ptr obj, action::action_type type) = 0;
};

class abstract_solve : public std::enable_shared_from_this<abstract_solve> //抽象的求解类定义了一些基础的接口和属性
{
protected:
	solve_notify* _notify;
protected:
	map_state_ptr _states;
	double _gama, _error;
public:
	abstract_solve(double gama, double error, solve_notify* notify);
	virtual ~abstract_solve();
	virtual void runing() = 0;
	virtual QWidget* make_widget() = 0;
	virtual QWidget* make_state_widget(state_ptr obj) = 0;
public:
	void set_states(map_state_ptr states);
public:
	template <class to_type>
	std::shared_ptr<to_type> to_ptr();
};
using solve_ptr = std::shared_ptr<abstract_solve>;

template<class to_type>
inline std::shared_ptr<to_type> abstract_solve::to_ptr()
{
	return std::dynamic_pointer_cast<to_type>(shared_from_this());
}


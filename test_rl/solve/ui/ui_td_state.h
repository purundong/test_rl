#pragma once

#include <QWidget>
#include "ui_ui_td_state.h"
#include "state/state.h"
#include "access.h"

class ui_td_state : public QWidget, public access
{
	Q_OBJECT

public:
	ui_td_state(state_ptr obj, QWidget *parent = nullptr);
	~ui_td_state();
public:
	virtual void begin_access(const action::action_type& type);
	virtual void end_access();
public:
	inline void set_error(const action::action_type& type, double error) { _action_error[type] = error; }
private:
	Ui::ui_td_stateClass ui;
	state_ptr _state;
protected:
	std::unordered_map<action::action_type, double> _action_error;
};

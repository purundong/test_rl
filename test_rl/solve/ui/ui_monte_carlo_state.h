#pragma once

#include <QWidget>
#include "ui_ui_monte_carlo_state.h"
#include "state/state.h"
#include "access.h"

class ui_monte_carlo_state : public QWidget, public access
{
	Q_OBJECT

public:
	ui_monte_carlo_state(state_ptr solve_obj, QWidget *parent = nullptr);
	~ui_monte_carlo_state();
public:
	virtual void begin_access(const action::action_type& type);
	virtual void end_access();
private:
	Ui::ui_monte_carlo_stateClass ui;
	state_ptr _state;
};

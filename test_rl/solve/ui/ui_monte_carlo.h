#pragma once

#include <QWidget>
#include "ui_ui_monte_carlo.h"
#include "../monte_carlo_solve.h"

class ui_monte_carlo : public QWidget
{
	Q_OBJECT

public:
	ui_monte_carlo(solve_ptr solve_obj, QWidget *parent = nullptr);
	~ui_monte_carlo();
public slots:
	void on_spinBox_episode_num_valueChanged(int i);
	void on_spinBox_episode_size_valueChanged(int i);
	void on_checkBox_episode_show_stateChanged(int state);
	void on_doubleSpinBox_epsilon_valueChanged(double d);
private:
	Ui::ui_monte_carloClass ui;
	monte_carlo_ptr _solve;
};

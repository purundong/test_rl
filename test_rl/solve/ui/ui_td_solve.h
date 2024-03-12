#pragma once

#include <QWidget>
#include "ui_ui_td_solve.h"
#include "../temporal_difference_solve.h"

class ui_td_solve : public QWidget
{
	Q_OBJECT

public:
	ui_td_solve(solve_ptr solve_obj, QWidget *parent = nullptr);
	~ui_td_solve();
public slots:
	void on_spinBox_episode_num_valueChanged(int i);
	void on_spinBox_episode_size_valueChanged(int i);
	void on_checkBox_episode_show_stateChanged(int state);
	void on_doubleSpinBox_epsilon_valueChanged(double d);
	void on_doubleSpinBox_lamda_valueChanged(double d);
private:
	Ui::ui_td_solveClass ui;
	temporal_difference_solve_ptr _solve;
};

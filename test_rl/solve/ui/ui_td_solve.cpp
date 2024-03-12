#include "ui_td_solve.h"

ui_td_solve::ui_td_solve(solve_ptr solve_obj, QWidget *parent)
	: QWidget(parent), _solve(solve_obj->to_ptr<temporal_difference_solve>())
{
	ui.setupUi(this);
	ui.spinBox_episode_num->blockSignals(true);
	ui.spinBox_episode_num->setValue(_solve->episode_num());
	ui.spinBox_episode_num->blockSignals(false);
	ui.spinBox_episode_size->blockSignals(true);
	ui.spinBox_episode_size->setValue(_solve->episode_size());
	ui.spinBox_episode_size->blockSignals(false);
	ui.checkBox_episode_show->blockSignals(true);
	ui.checkBox_episode_show->setChecked(_solve->episode_show());
	ui.checkBox_episode_show->blockSignals(false);
	ui.doubleSpinBox_epsilon->blockSignals(true);
	ui.doubleSpinBox_epsilon->setValue(_solve->epsilon());
	ui.doubleSpinBox_epsilon->blockSignals(false);
	ui.doubleSpinBox_lamda->blockSignals(true);
	ui.doubleSpinBox_lamda->setValue(_solve->lamda());
	ui.doubleSpinBox_lamda->blockSignals(false);
}

ui_td_solve::~ui_td_solve()
{
}

void ui_td_solve::on_spinBox_episode_num_valueChanged(int i)
{
	_solve->set_episode_num(i);
}

void ui_td_solve::on_spinBox_episode_size_valueChanged(int i)
{
	_solve->set_episode_size(i);
}

void ui_td_solve::on_checkBox_episode_show_stateChanged(int state)
{
	_solve->set_episode_show(state == Qt::Checked);
}

void ui_td_solve::on_doubleSpinBox_epsilon_valueChanged(double d)
{
	_solve->set_epsilon(d);
}

void ui_td_solve::on_doubleSpinBox_lamda_valueChanged(double d)
{
	_solve->set_lamda(d);
}
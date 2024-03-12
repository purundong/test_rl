#pragma once
#include <QtWidgets/QWidget>
#include <memory>
#include "ui_test_rl.h"
#include <QMenu>
#include "solve/abstract_solve.h"
#include <boost/thread.hpp>
#include "concrete_solve_notify.h"

class test_rl : public QWidget, public solve_notify
{
	Q_OBJECT

public:
	test_rl(QWidget* parent = nullptr);
	~test_rl();
	void init_tableWidget();
public slots:
	void menu_set_rewards();
	void menu_set_transfers();
	void menu_state_info();
	void on_pushButton_solve_clicked();
	void on_pushButton_make_state_clicked();
	void on_comboBox_mode_currentIndexChanged(int index);
	void on_horizontalSlider_valueChanged(int value);
	void on_tableWidget_customContextMenuRequested(QPoint point);
signals:
	void sig_solve_start();
	void sig_solve_end();
	void sig_state_change(state_ptr obj, action::action_type type);
public slots:
	void sot_solve_start();
	void sot_solve_end();
	void sot_state_change(state_ptr obj, action::action_type type);
public:
	virtual void on_solve_start();
	virtual void on_solve_end();
	virtual void on_state_change(state_ptr obj, action::action_type type);
private:
	Ui::test_rlClass ui;
private:
	int _magni_fication = 10;
	QMenu* _menu;
	map_state_ptr _states;
	solve_ptr _solve;
	QHBoxLayout* _layout{ nullptr };
	QWidget* _widget{ nullptr };
	std::unique_ptr<boost::thread> _thread;
};

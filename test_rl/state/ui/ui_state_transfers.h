#pragma once

#include <QDialog>
#include "ui_ui_state_transfers.h"
#include "../state.h"

class ui_state_transfers : public QDialog //设置状态转移的界面
{
	Q_OBJECT

public:
	ui_state_transfers(state_ptr s_obj, map_state_ptr map_state_obj, QWidget *parent = nullptr);
	~ui_state_transfers();
public slots:
	void on_pushButton_add_clicked();
	void on_pushButton_del_clicked();
	state_ptr get_state() { return _state; }
private:
	Ui::ui_state_transfersClass ui;
	state_ptr _state;
	map_state_ptr _map_state;
};

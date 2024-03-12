#pragma once

#include <QDialog>
#include "ui_ui_state_info.h"
#include "../state.h"

class ui_state_info : public QDialog //显示状态信息的界面
{
	Q_OBJECT

public:
	ui_state_info(state_ptr state_obj, QWidget *parent = nullptr);
	~ui_state_info();

private:
	Ui::ui_state_infoClass ui;
	state_ptr _state;
};

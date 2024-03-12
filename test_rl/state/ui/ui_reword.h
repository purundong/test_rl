#pragma once

#include <QDialog>
#include "ui_ui_reword.h"
#include "../state.h"

class ui_reword : public QDialog //设置状态的奖励的界面
{
	Q_OBJECT

public:
	ui_reword(state_ptr state_obj, QWidget *parent = nullptr);
	~ui_reword();
public slots:
	void on_pushButton_add_clicked();
	void on_pushButton_del_clicked();
private:
	Ui::ui_rewordClass ui;
	state_ptr _state;
};

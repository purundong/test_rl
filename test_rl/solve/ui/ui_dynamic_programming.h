#pragma once
#include <QWidget>
#include "ui_ui_dynamic_programming.h"
#include "../dynamic_programming_solve.h"

class ui_dynamic_programming : public QWidget
{
	Q_OBJECT

public:
	ui_dynamic_programming(solve_ptr solve_obj, QWidget *parent = nullptr);
	~ui_dynamic_programming();
public slots:
	void on_spinBox_evaluate_bout_valueChanged(int i);
private:
	Ui::ui_dynamic_programmingClass ui;
	dynamic_programming_ptr _solve;
};

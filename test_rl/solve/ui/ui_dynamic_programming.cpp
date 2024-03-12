#include "ui_dynamic_programming.h"
ui_dynamic_programming::ui_dynamic_programming(solve_ptr solve_obj, QWidget *parent)
	: QWidget(parent),
	_solve{ solve_obj->to_ptr<dynamic_programming_solve>()}
{
	ui.setupUi(this);
	ui.spinBox_evaluate_bout->blockSignals(true);
	ui.spinBox_evaluate_bout->setValue(_solve->evaluate_bout());
	ui.spinBox_evaluate_bout->blockSignals(false);
}

ui_dynamic_programming::~ui_dynamic_programming()
{}

void ui_dynamic_programming::on_spinBox_evaluate_bout_valueChanged(int i)
{
	_solve->set_evaluate_bout(i);
}

#include "ui_state_info.h"
#include "delegate.h"

ui_state_info::ui_state_info(state_ptr state_obj, QWidget *parent)
	: QDialog(parent),
	_state(state_obj)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("%1:的奖励").arg(QString::fromStdString(_state->get_name())));
	ui.tableWidget_policy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_action_value->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	spinbox_double* double_obj = new spinbox_double(-100000, 100000, 3);
	text* no_text = new text(false);
	ui.tableWidget_policy->setItemDelegateForColumn(0, no_text);
	ui.tableWidget_policy->setItemDelegateForColumn(1, no_text);
	ui.tableWidget_policy->setItemDelegateForColumn(2, double_obj);

	ui.tableWidget_action_value->setItemDelegateForColumn(0, no_text);
	ui.tableWidget_action_value->setItemDelegateForColumn(1, no_text);
	ui.tableWidget_action_value->setItemDelegateForColumn(2, double_obj);

	auto& polocy = _state->policy();
	ui.tableWidget_policy->setRowCount(polocy.size());
	for (int r = 0;auto& [type, p] : polocy)
	{
		ui.tableWidget_policy->setItem(r, 0, new QTableWidgetItem(QString::number(type)));
		ui.tableWidget_policy->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(action::get_name(type).data())));
		ui.tableWidget_policy->setItem(r, 2, new QTableWidgetItem(QString::number(p)));
		++r;
	}
	auto actions = _state->map_action();
    ui.tableWidget_action_value->setRowCount(actions->size());
	for (int r = 0; auto & [type, a] : *actions)
	{
		ui.tableWidget_action_value->setItem(r, 0, new QTableWidgetItem(QString::number(type)));
		ui.tableWidget_action_value->setItem(r, 1, new QTableWidgetItem(QString::fromLocal8Bit(a->get_name().data())));
		ui.tableWidget_action_value->setItem(r, 2, new QTableWidgetItem(QString::number(a->value())));
		++r;
	}
	ui.label_state_value->setText(QString::number(_state->value()));
}

ui_state_info::~ui_state_info()
{
}

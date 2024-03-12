#include "ui_reword.h"
#include "delegate.h"

ui_reword::ui_reword(state_ptr state_obj, QWidget* parent)
	: QDialog(parent),
	_state{ state_obj }
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("%1:的奖励").arg(QString::fromStdString(_state->get_name())));
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	spinbox_double* double_obj = new spinbox_double(-100000, 100000, 3);
	text* no_text = new text(false);
	ui.tableWidget->setItemDelegateForColumn(0, no_text);
	ui.tableWidget->setItemDelegateForColumn(1, double_obj);
	auto& rewards = _state->rewards();
	ui.tableWidget->setRowCount(rewards.size());
	for (int row = 0; auto & [r, p]: rewards)
	{
		ui.tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(r)));
		ui.tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(p)));
	}
}

ui_reword::~ui_reword()
{
	auto& rewards = _state->rewards();
	rewards.clear();
	for (int i = 0 ; i < ui.tableWidget->rowCount();++i)
		rewards[ui.tableWidget->item(i, 0)->text().toDouble()] = ui.tableWidget->item(i, 1)->text().toDouble();
}

void ui_reword::on_pushButton_add_clicked()
{
	auto row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);
	ui.tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(0)));
	ui.tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(0)));
}

void ui_reword::on_pushButton_del_clicked()
{
	auto row = ui.tableWidget->currentRow();
	if (row < 0)
		return;
	//auto& rewards = _state->rewards();
	//auto key = ui.tableWidget->item(row, 0)->text().toDouble();
	//rewards.erase(key);
	ui.tableWidget->removeRow(row);
}

#include "test_rl.h"
#include <QScrollBar>
#include <QMenu>
#include <QMessageBox>
#include "state/ui/ui_state.h"
#include "state/ui/ui_state_info.h"
#include "solve/ui/access.h"
#include "solve/dynamic_programming_solve.h"
#include "solve/monte_carlo_solve.h"
#include "solve/temporal_difference_solve.h"
#include "solve/on_policy_qlearning_solve.h"

test_rl::test_rl(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init_tableWidget();
	qRegisterMetaType<state_ptr>("state_ptr");
	qRegisterMetaType<action::action_type>("action::action_type");
	connect(this, SIGNAL(sig_solve_start()), this, SLOT(sot_solve_start()), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(sig_solve_end()), this, SLOT(sot_solve_end()), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(sig_state_change(state_ptr, action::action_type)), this, SLOT(sot_state_change(state_ptr, action::action_type)), Qt::BlockingQueuedConnection);
	_layout = new QHBoxLayout;
	_layout->setContentsMargins(0, 0, 0, 0);
	ui.groupBox_mode->setLayout(_layout);
	on_comboBox_mode_currentIndexChanged(0);
}

test_rl::~test_rl()
{
}

void test_rl::init_tableWidget()
{
	ui.tableWidget->verticalHeader()->hide();
	ui.tableWidget->horizontalHeader()->hide();
	ui.tableWidget->viewport()->installEventFilter(this);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(_magni_fication * 10);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(_magni_fication * 10);
	ui.tableWidget->setColumnWidth(0, 10);
	ui.tableWidget->setRowHeight(0, 10);
	ui.tableWidget->verticalScrollBar()->installEventFilter(this);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	_menu = new QMenu(ui.tableWidget);

	QAction* menu_action = new QAction(QString::fromLocal8Bit("设置奖励"));
	connect(menu_action, SIGNAL(triggered()), this, SLOT(menu_set_rewards()));
	_menu->addAction(menu_action);
	menu_action = new QAction(QString::fromLocal8Bit("设置状态转移"));
	connect(menu_action, SIGNAL(triggered()), this, SLOT(menu_set_transfers()));
	_menu->addAction(menu_action);
	menu_action = new QAction(QString::fromLocal8Bit("查看状态信息"));
	connect(menu_action, SIGNAL(triggered()), this, SLOT(menu_state_info()));
	_menu->addAction(menu_action);
	ui.horizontalSlider->blockSignals(true);
	ui.horizontalSlider->setPageStep(1);
	ui.horizontalSlider->setValue(_magni_fication);
	ui.horizontalSlider->setMinimum(1);
	ui.horizontalSlider->setMaximum(20);
	ui.horizontalSlider->blockSignals(false);

}

void test_rl::menu_set_rewards()
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	if (row < 0 || col < 0) return;
	auto* s_ui = dynamic_cast<ui_state*>(ui.tableWidget->cellWidget(row, col));
	if (s_ui == nullptr) return;
	s_ui->get_state()->make_set_reword()->exec();
	s_ui->show_state();
}

void test_rl::menu_set_transfers()
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	if (row < 0 || col < 0) return;
	auto* s_ui = dynamic_cast<ui_state*>(ui.tableWidget->cellWidget(row, col));
	if (s_ui == nullptr) return;
	s_ui->get_state()->make_set_state_transfers(_states)->exec();
}

void test_rl::menu_state_info()
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	if (row < 0 || col < 0) return;
	auto* s_ui = dynamic_cast<ui_state*>(ui.tableWidget->cellWidget(row, col));
	if (s_ui == nullptr) return;
	s_ui->get_state()->make_show_state_info()->exec();
}

void test_rl::on_pushButton_solve_clicked()
{
	if (_states == nullptr || (**_states).empty()) {
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("没有生成状态"), QMessageBox::Ok);
		return;
	}
	if (_thread != nullptr) {
		_thread->interrupt();
		_thread->join();
		_thread = nullptr;
	}
	else {
		//on_comboBox_mode_currentIndexChanged(ui.comboBox_mode->currentIndex());
		auto fun = [&]() {_solve->set_states(_states), _solve->runing(); };
		_thread = std::make_unique<boost::thread>(fun);
	}
}

void test_rl::on_pushButton_make_state_clicked()
{
	int r_size = ui.spinBox_row->value();
	int c_size = ui.spinBox_col->value();
	int trap = ui.spinBox_trap->value();
	int target = ui.spinBox_target->value();
	int all = r_size * c_size;

	if (all < (trap + target)) {
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("陷阱和终点之和不得大于状态数"), QMessageBox::Ok);
		return;
	}

	ui.tableWidget->setRowCount(0);
	ui.tableWidget->setColumnCount(0);

	ui.tableWidget->setRowCount(r_size);
	ui.tableWidget->setColumnCount(c_size);
	_states = std::make_shared<map_state>(r_size, c_size, trap, target);
	for (auto& s_key : **_states)
	{
		auto* s_ui = new ui_state(s_key.second);
		ui.tableWidget->setCellWidget(s_key.second->x(), s_key.second->y(), s_ui);
		s_ui->show_state();
	}
}

void test_rl::on_comboBox_mode_currentIndexChanged(int index)
{
	switch (index)
	{
	case 0:
	{
		_solve = std::make_shared<dynamic_programming_solve>(ui.doubleSpinBox_gama->value(), ui.doubleSpinBox_error->value(), this);
	}break;
	case 1:
	{
		_solve = std::make_shared<monte_carlo_solve>(ui.doubleSpinBox_gama->value(), ui.doubleSpinBox_error->value(), this);
	}break;
	case 2:
	{
		_solve = std::make_shared<temporal_difference_solve>(ui.doubleSpinBox_gama->value(), ui.doubleSpinBox_error->value(), this);
	}break;
	case 3:
	{
		_solve = std::make_shared<on_policy_qlearning_solve>(ui.doubleSpinBox_gama->value(), ui.doubleSpinBox_error->value(), this);
	}break;
	case 4:
	{
		_solve = std::make_shared<temporal_difference_solve>(ui.doubleSpinBox_gama->value(), ui.doubleSpinBox_error->value(), this);
	}break;
	}
	if (_widget != nullptr) delete _widget;
	//_layout->removeWidget(_widget);
	_widget = _solve->make_widget();
	_layout->addWidget(_widget);
}

void test_rl::on_horizontalSlider_valueChanged(int value)
{
	if (_magni_fication == value) return;
	_magni_fication = value;
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(_magni_fication * 10);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(_magni_fication * 10);
}

void test_rl::on_tableWidget_customContextMenuRequested(QPoint point)
{
	_menu->exec(QCursor::pos());
}

void test_rl::on_solve_start()
{
	sig_solve_start();
}

void test_rl::on_solve_end()
{
	sig_solve_end();
}

void test_rl::on_state_change(state_ptr obj, action::action_type type)
{
	sig_state_change(obj, type);
}

void test_rl::sot_solve_start()
{
	for (auto& s_key : **_states)
	{
		ui.tableWidget->removeCellWidget(s_key.second->x(), s_key.second->y());
		ui.tableWidget->setCellWidget(s_key.second->x(), s_key.second->y(), _solve->make_state_widget(s_key.second));
	}
}

void test_rl::sot_solve_end()
{
	for (auto& s_key : **_states)
	{
		ui.tableWidget->removeCellWidget(s_key.second->x(), s_key.second->y());
		ui.tableWidget->setCellWidget(s_key.second->x(), s_key.second->y(), new ui_state(s_key.second));
	}
}

void test_rl::sot_state_change(state_ptr obj, action::action_type type)
{
	static state_ptr old_obj = nullptr;
	if (old_obj != nullptr) {
		dynamic_cast<access*>(ui.tableWidget->cellWidget(old_obj->x(), old_obj->y()))->end_access();
		old_obj = obj;
		dynamic_cast<access*>(ui.tableWidget->cellWidget(old_obj->x(), old_obj->y()))->begin_access(type);
	}
	else {
		old_obj = obj;
		dynamic_cast<access*>(ui.tableWidget->cellWidget(old_obj->x(), old_obj->y()))->begin_access(type);
	}
}

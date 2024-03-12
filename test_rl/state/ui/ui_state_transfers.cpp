#include "ui_state_transfers.h"
#include "delegate.h"
ui_state_transfers::ui_state_transfers(state_ptr s_obj, map_state_ptr map_state_obj, QWidget* parent)
	: QDialog(parent),
	_state(s_obj),
	_map_state{ map_state_obj }
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("%1:的状态转移").arg(QString::fromStdString(_state->get_name())));
	ui.treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
	QStringList str;
	for (auto& key : **_map_state)
		str << QString::fromStdString(key.first);
	combobox* box = new combobox(str);
	auto actions = _state->map_action();
	ui.treeWidget->setItemDelegateForColumn(2, box);
	auto on_text = new text(false);
	ui.treeWidget->setItemDelegateForColumn(0, on_text);
	ui.treeWidget->setItemDelegateForColumn(1, on_text);
	spinbox_double* double_obj = new spinbox_double(-100000, 100000, 3);
	ui.treeWidget->setItemDelegateForColumn(3, double_obj);

	for (auto& [type, a] : *actions)
	{
		auto root_item = new QTreeWidgetItem;
		root_item->setText(0, QString::number(type));
		root_item->setText(1, QString::fromLocal8Bit(a->get_name().data()));
		ui.treeWidget->addTopLevelItem(root_item);
		auto& transfer = a->state_transfer();
		for (auto& [name, s] : transfer)
		{
			auto item = new QTreeWidgetItem;
			item->setText(2, QString::fromStdString(name));
			item->setText(3, QString::number(std::get<1>(s)));
			root_item->addChild(item);
		}
	}
}

ui_state_transfers::~ui_state_transfers()
{
	auto actions = _state->map_action();
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); i++)
	{
		auto root_item = ui.treeWidget->topLevelItem(i);
		auto type = action::action_type(root_item->text(0).toInt());

		if (auto a_key = (*actions).find(type); a_key != actions->end()) {
			auto& transfer = a_key->second->state_transfer();
			transfer.clear();
			for (int j = 0; j < root_item->childCount(); j++)
			{
				auto item = root_item->child(j);
				auto name = item->text(2).toStdString();
				if (auto s_key = (**_map_state).find(name); s_key != (**_map_state).end()) {
					auto value = item->text(3).toDouble();
					transfer[name] = std::make_tuple(s_key->second, value);
				}
			}
		}
	}
}

void ui_state_transfers::on_pushButton_add_clicked()
{
	auto c_item = ui.treeWidget->currentItem();
	if (c_item == nullptr)return;
	c_item = c_item->parent() == nullptr ? c_item : c_item->parent();
	auto item = new QTreeWidgetItem;
	item->setText(2, QString::fromStdString((**_map_state).begin()->first));
	item->setText(3, QString::number(0.0));
}

void ui_state_transfers::on_pushButton_del_clicked()
{
	auto item = ui.treeWidget->currentItem();
	if (item->parent() == nullptr)return;
	auto root_item = item->parent();
	root_item->removeChild(item);
}

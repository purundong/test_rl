#include "delegate.h"
#include <QStringlist>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSet>
#include <QComboBox>
#include <QString>

//spinbox_double
QWidget* spinbox_double::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
	editor->setFrame(false);
	editor->setDecimals(_decimals);
	editor->setRange(_min, _max);
	editor->setValue(index.data().toDouble());
	editor->setPrefix(_prefix);
	return editor;
}

void spinbox_double::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QDoubleSpinBox* editor1 = dynamic_cast<QDoubleSpinBox*>(editor);
	auto str = index.data().toString();
	if (_prefix.isEmpty())
		editor1->setValue(index.data().toDouble());
	else {
		auto str = index.data().toString();
		editor1->setValue(str.split(_delimiter)[1].toDouble());
	}
}

void spinbox_double::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QDoubleSpinBox* editor1 = dynamic_cast<QDoubleSpinBox*>(editor);
	auto v = editor1->value();
	if (_delimiter.isEmpty())
		model->setData(index, v);
	else
		model->setData(index, _prefix + QString::number(v));
}

void spinbox_double::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

spinbox_double::spinbox_double(const double& min, const double& max, const unsigned int& decimals, const QString& prefix, const QString& delimiter) :
	_min{ min }, _max{ max }, _decimals{ decimals }, _prefix{ prefix }, _delimiter{ delimiter }
{
}
//spinbox_double -end

//spinbox_int
QWidget* spinbox_int::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSpinBox* editor = new QSpinBox(parent);
	editor->setFrame(false);
	editor->setRange(_min, _max);
	editor->setValue(index.data().toInt());
	return editor;
}

void spinbox_int::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QSpinBox* editor1 = dynamic_cast<QSpinBox*>(editor);
	editor1->setValue(index.data().toInt());
}

void spinbox_int::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QSpinBox* editor1 = dynamic_cast<QSpinBox*>(editor);
	editor1->interpretText();
	auto v = editor1->value();
	model->setData(index, v);
}

void spinbox_int::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

spinbox_int::spinbox_int(const int& min, const int& max) :
	_min{ min }, _max{ max }
{
}
//spinbox_int -end
// 
//text
text::text(const bool enabled, QLineEdit::EchoMode type) : _enabled{ enabled }, _type{ type }
{
}

void text::paint(QPainter* painter,
	const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem viewOption(option);
	//高亮显示与普通显示时的前景色一致（即选中行和为选中时候的文字颜色一样）
	viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::ForegroundRole).value<QColor>());
	QStyledItemDelegate::paint(painter, viewOption, index);
}


QWidget* text::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QLineEdit* edit = new QLineEdit(parent);
	edit->setEnabled(_enabled);
	edit->setEchoMode(_type);
	return edit;
}

void text::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QLineEdit* edit = dynamic_cast<QLineEdit*>(editor);
	edit->setText(index.data().toString());
}

void text::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QLineEdit* edit = dynamic_cast<QLineEdit*>(editor);
	auto a = edit->text();
	model->setData(index, edit->text());
}
void text::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
//text -end

//combobox
combobox::combobox(const QStringList& data) :_data{ data }
{
	for (int i = 0; i < data.size(); ++i)
		_index.insert(data[i], i);
}

QWidget* combobox::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QComboBox* edit = new QComboBox(parent);
	edit->addItems(_data);
	return edit;
}

void combobox::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QComboBox* edit = dynamic_cast<QComboBox*>(editor);
	QString dateStr = index.model()->data(index).toString();
	if (auto key = _index.find(dateStr); key != _index.end())
		edit->setCurrentIndex(key.value());
	else
		edit->setCurrentIndex(0);
}

void combobox::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* edit = dynamic_cast<QComboBox*>(editor);
	int curr_index = edit->currentIndex();
	if (curr_index < 0 || curr_index >= _data.size())return;
	model->setData(index, _data[curr_index]);
}

void combobox::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

//combobox -end
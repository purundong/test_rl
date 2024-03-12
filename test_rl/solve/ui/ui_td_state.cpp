#include "ui_td_state.h"

ui_td_state::ui_td_state(state_ptr obj, QWidget *parent)
	: QWidget(parent), _state{ obj }
{
	ui.setupUi(this);
}

ui_td_state::~ui_td_state()
{
}

void ui_td_state::begin_access(const action::action_type& type)
{
	QPalette pal1(palette());
	setAutoFillBackground(true);
	pal1.setColor(QPalette::Base, QColor(0, 153, 153));
	setPalette(pal1);
	auto& access = _action_access[type];
	auto& err = _action_error[type];
	++access;
	switch (type)
	{
	case action::action_type::up: {ui.label_up->setText(QString::number(access)); ui.label_up_err->setText(QString::number(err)); } break;
	case action::action_type::right: {ui.label_right->setText(QString::number(access)); ui.label_right_err->setText(QString::number(err)); }break;
	case action::action_type::down: {ui.label_dow->setText(QString::number(access)); ui.label_dow_err->setText(QString::number(err)); }break;
	case action::action_type::left: {ui.label_left->setText(QString::number(access)); ui.label_left_err->setText(QString::number(err)); } break;
	case action::action_type::fixed: {ui.label_fixed->setText(QString::number(access)); ui.label_fixed_err->setText(QString::number(err)); }break;
	}
}

void ui_td_state::end_access()
{
	QPalette pal1(palette());
	setAutoFillBackground(true);
	pal1.setColor(QPalette::Base, _state->get_color());
	setPalette(pal1);
}

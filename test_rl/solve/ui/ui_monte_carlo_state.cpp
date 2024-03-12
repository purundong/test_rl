#include "ui_monte_carlo_state.h"

ui_monte_carlo_state::ui_monte_carlo_state(state_ptr state_obj, QWidget *parent)
	: QWidget(parent),
	_state{ state_obj }
{
	ui.setupUi(this);
}

ui_monte_carlo_state::~ui_monte_carlo_state()
{
}

void ui_monte_carlo_state::begin_access(const action::action_type& type)
{
	QPalette pal1(palette());
	setAutoFillBackground(true);
	pal1.setColor(QPalette::Base, QColor(0, 153, 153));
	setPalette(pal1);
	auto& access = _action_access[type];
	++access;
	switch (type)
	{
	case action::action_type::up: {ui.label_up->setText(QString::number(access)); } break;
	case action::action_type::right: {ui.label_right->setText(QString::number(access)); }break;
	case action::action_type::down: {ui.label_dow->setText(QString::number(access)); }break;
	case action::action_type::left: {ui.label_left->setText(QString::number(access)); } break;
	case action::action_type::fixed: {ui.label_fixed->setText(QString::number(access)); }break;
	}
}

void ui_monte_carlo_state::end_access()
{
	QPalette pal1(palette());
	setAutoFillBackground(true);
	pal1.setColor(QPalette::Base, _state->get_color());
	setPalette(pal1);
}

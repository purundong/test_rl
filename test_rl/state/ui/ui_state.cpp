#include "ui_state.h"
#include <QPainter>
#include <QGraphicsOpacityEffect>

ui_state::ui_state(state_ptr s_obj, QWidget* parent)
	: QWidget(parent), _state(s_obj)
{
	ui.setupUi(this);
	ui.widget_up->setAttribute(Qt::WA_WState_WindowOpacitySet);
	ui.widget_dow->setAttribute(Qt::WA_WState_WindowOpacitySet);
	ui.widget_right->setAttribute(Qt::WA_WState_WindowOpacitySet);
	ui.widget_left->setAttribute(Qt::WA_WState_WindowOpacitySet);
	ui.widget_fixed->setAttribute(Qt::WA_WState_WindowOpacitySet);
	show_state();
}

ui_state::ui_state(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	show_state();
}

ui_state::~ui_state()
{
}

void ui_state::resizeEvent(QResizeEvent* event)
{
	show_state();
}

void ui_state::set_widget_color(const QColor& color)
{
	QPalette pal1(palette());
	setAutoFillBackground(true);
	pal1.setColor(QPalette::Base, color);
	setPalette(pal1);
}

void ui_state::show_state()
{
	{
		ui.widget_up->setAutoFillBackground(true);
		QImage image;
		QPalette palette;
		image.load(":/test_rl/R-C.png"); // 指定图片所在位置及图片名
		QMatrix matrix;
		matrix.rotate(-90.0);
		palette.setBrush(this->backgroundRole(), QBrush(image.transformed(matrix, Qt::FastTransformation).scaled(ui.widget_up->size())));
		ui.widget_up->setPalette(palette);
		auto graph = new QGraphicsOpacityEffect(ui.widget_up);
		graph->setOpacity(_state->policy(action::action_type::up));
		ui.widget_up->setGraphicsEffect(graph);
	}

	{
		ui.widget_right->setAutoFillBackground(true);
		QImage image;
		QPalette palette;
		image.load(":/test_rl/R-C.png"); // 指定图片所在位置及图片名
		palette.setBrush(this->backgroundRole(), QBrush(image.scaled(ui.widget_right->size())));
		ui.widget_right->setPalette(palette);
		auto graph = new QGraphicsOpacityEffect(ui.widget_up);
		graph->setOpacity(_state->policy(action::action_type::right));
		ui.widget_right->setGraphicsEffect(graph);
	}

	{
		ui.widget_dow->setAutoFillBackground(true);
		QImage image;
		QPalette palette;
		image.load(":/test_rl/R-C.png"); // 指定图片所在位置及图片名
		QMatrix matrix;
		matrix.rotate(90.0);
		palette.setBrush(this->backgroundRole(), QBrush(image.transformed(matrix, Qt::FastTransformation).scaled(ui.widget_dow->size())));
		ui.widget_dow->setPalette(palette);
		auto graph = new QGraphicsOpacityEffect(ui.widget_up);
		graph->setOpacity(_state->policy(action::action_type::down));
		ui.widget_dow->setGraphicsEffect(graph);
	}

	{
		ui.widget_left->setAutoFillBackground(true);
		QImage image;
		QPalette palette;
		image.load(":/test_rl/R-C.png"); // 指定图片所在位置及图片名
		QMatrix matrix;
		matrix.rotate(180);
		palette.setBrush(this->backgroundRole(), QBrush(image.transformed(matrix, Qt::FastTransformation).scaled(ui.widget_left->size())));
		ui.widget_left->setPalette(palette);
		auto graph = new QGraphicsOpacityEffect(ui.widget_up);
		graph->setOpacity(_state->policy(action::action_type::left));
		ui.widget_left->setGraphicsEffect(graph);
	}

	{
		ui.widget_fixed->setAutoFillBackground(true);
		QImage image;
		QPalette palette;
		image.load(":/test_rl/fixed.png"); // 指定图片所在位置及图片名
		palette.setBrush(this->backgroundRole(), QBrush(image.scaled(ui.widget_fixed->size())));
		ui.widget_fixed->setPalette(palette);
		auto graph = new QGraphicsOpacityEffect(ui.widget_up);
		graph->setOpacity(_state->policy(action::action_type::fixed));
		ui.widget_fixed->setGraphicsEffect(graph);
	}
	set_widget_color(_state->get_color());
}

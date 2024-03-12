#pragma once

#include <QWidget>
#include "ui_ui_state.h"
#include "../state.h"

class ui_state : public QWidget
{
	Q_OBJECT

public:
	ui_state(state_ptr s_obj, QWidget* parent = nullptr);
	ui_state(QWidget* parent = nullptr);
	virtual ~ui_state();
	void show_state();
	state_ptr get_state() {return _state;}
protected:
	void resizeEvent(QResizeEvent* event);
private:
	void set_widget_color(const QColor& color);
private:
	Ui::ui_stateClass ui;
	state_ptr _state;
};

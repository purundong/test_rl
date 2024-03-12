#pragma once
#include <unordered_map>
#include "state/action.h"

class access
{
protected:
	std::unordered_map<action::action_type, int> _action_access;
public:
	virtual void begin_access(const action::action_type& type) = 0;
	virtual void end_access() = 0;
};


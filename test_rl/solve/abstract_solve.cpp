#include "abstract_solve.h"

abstract_solve::abstract_solve(double gama, double error, solve_notify* notify) :
	_gama{ gama }, _error{ error }, _notify{ notify }
{
}

abstract_solve::~abstract_solve()
{
}

void abstract_solve::set_states(map_state_ptr states)
{
	_states = states;
}


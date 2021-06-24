#include "StateMachine.h"
#include "EmptyState.h"
#include "State.h"

StateMachine::StateMachine() {
	empty_state = new EmptyState();
	state = empty_state;
}

void StateMachine::changeState(std::string new_state_name)
{
	state->exit();
	state = states.at(new_state_name);
	state->enter();
}

State* StateMachine::getState()
{
	return state;
}

void StateMachine::addState(std::string state_name, State* new_state)
{
	states.emplace(state_name, new_state);
}

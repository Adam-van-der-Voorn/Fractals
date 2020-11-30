#pragma once
#include <unordered_map>
#include <memory>
class State;
class EmptyState;

class StateMachine
{
public:
	StateMachine();
	/**
	changes the state to the state with the given name.
	calls the exit function on the old state, and the enter function on the new one.
	\param new_state_name the name of the state you want to change to
	**/
	void changeState(std::string new_state_name);

	/**
	\return the current active state
	**/
	State* getState();

	/**
	adds a state to the collection of states, and attaches a name to it.
	\param state_name the name of the state
	\param state the state itself
	**/
	void addState(std::string state_name, State* state);

private:
	State* empty_state;
	State* state;
	std::unordered_map<std::string, State*> states;
};
#pragma once
#include <unordered_map>
class State;
class EmptyState;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();
	void changeState(std::string new_state_name);
	State* getState();
	void addState(std::string state_name, State* state);

private:
	EmptyState* empty_state;
	State* state;
	std::unordered_map<std::string, State*> states;
};
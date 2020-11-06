#pragma once
#include <unordered_set>
class Observer;

class Subject
{
public:
	void addObserver(Observer*);
	void notify(int);
private:
	std::unordered_set<Observer*> observers;
};


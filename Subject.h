#pragma once
class Observer;

class Subject
{
public:
	virtual void addObserver(Observer* observer) = 0;
	virtual void removeObserver(Observer* observer) = 0;
};
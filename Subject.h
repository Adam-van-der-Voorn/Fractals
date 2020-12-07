#pragma once
#include "Observer.h"
#include <list>

template <class T>
class Subject
{
public:
	virtual ~Subject() {}
	void addObserver(Observer<T>* observer)
	{
		observers.push_back(observer);
	}
	void removeObserver(Observer<T>* observer)
	{
		observers.remove(observer);
	}
	void notifyAll(int event_num)
	{
		for (Observer<T>* observer : observers) {
			observer->onNotify(static_cast<T*>(this), event_num);
		}
	}
private:
	std::list<Observer<T>*> observers;
};
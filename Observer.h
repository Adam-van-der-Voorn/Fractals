#pragma once
class Observer
{
public:
	virtual void onNotify(int event_num) = 0;
};


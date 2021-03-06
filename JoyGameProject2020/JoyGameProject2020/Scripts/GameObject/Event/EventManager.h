#pragma once
#include <vector>

#include "iEvent.h"

class EventManager
{
public:
	EventManager() {};
	EventManager(const EventManager&) {};
	EventManager&operator=(const EventManager&) {};
	static EventManager&Instance();
	~EventManager();

	void initialize();
	void update();
	void Clear();

	void SetEvent(iEvent* event);
	void EraseEvent(int place);


private:
	std::vector<iEvent*>playevent;

};
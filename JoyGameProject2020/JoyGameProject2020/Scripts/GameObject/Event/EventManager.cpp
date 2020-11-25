#include "EventManager.h"
#include <memory>

EventManager & EventManager::Instance()
{
	static std::unique_ptr<EventManager>instance(new EventManager());
	return *instance;
}

EventManager::~EventManager()
{
	for (int i=0;i<playevent.size();)
	{
		delete(playevent[i]);
		++i;
	}
	playevent.clear();
}

void EventManager::initialize()
{
}

void EventManager::update()
{
	for (int i = 0; i < playevent.size();)
	{
		
		if (playevent[i]->IsEnd())
		{
			EraseEvent(i);
			continue;
		}
		playevent[i]->update();
		++i;
	}
}

void EventManager::SetEvent(iEvent* event)
{
	playevent.emplace_back(event);
	event->initialize();
}

void EventManager::EraseEvent(int place)
{
	delete(playevent[place]);
	playevent[place] = playevent.back();
	playevent.pop_back();
}

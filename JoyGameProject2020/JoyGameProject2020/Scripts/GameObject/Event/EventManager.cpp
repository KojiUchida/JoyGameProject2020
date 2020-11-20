#include "EventManager.h"
#include <memory>

EventManager & EventManager::Instance()
{
	static std::unique_ptr<EventManager>instance(new EventManager());
	return *instance;
}

EventManager::~EventManager()
{
	for (int i = 0; i < playevent.size(); ++i)
	{
		EraseEvent(*playevent[i]);
	}
}

void EventManager::initialize()
{
}

void EventManager::update()
{
	for (int i = 0; i < playevent.size(); ++i)
	{
		playevent[i]->update();
	}

	for (int i = 0; i < playevent.size(); ++i)
	{
		if (playevent[i]->IsEnd)
		{
			EraseEvent(*playevent[i]);
		}
	}
}

void EventManager::SetEvent(iEvent* event)
{
	playevent.emplace_back(event);
}

void EventManager::EraseEvent(iEvent* event)
{
	auto itr = std::find(playevent.begin(), playevent.end(), event);
	delete(*itr);
	playevent.erase(itr);
}

#pragma once

class iEvent
{
public:
	virtual void initialize() = 0;
	virtual void update()  = 0;

	virtual bool IsEnd()=0;

	bool isEnd;

};
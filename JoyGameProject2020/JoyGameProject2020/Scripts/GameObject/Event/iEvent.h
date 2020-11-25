#pragma once
#include <memory>
#include <vector>

#include "Math/Vector3.h"
#include "Utility/Timer.h"

class iEvent
{
public:
	virtual ~iEvent() {};
	virtual void initialize() = 0;
	virtual void update()  = 0;

	virtual bool IsEnd()=0;

	bool isEnd;
};
#include "ServerTimeHandler.h"

namespace Prototype
{
	const float ServerTimeHandler::TICK_DELTA_TIME = static_cast<float>(1000.0f / TICKS_PER_SECOND);

	void ServerTimeHandler::reset() 
	{ 
		TimeHandler::reset();
		tick = 0;
	}

	void ServerTimeHandler::nextTick()
	{
		tick++;
	}
};


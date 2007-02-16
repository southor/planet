#include "ServerTimeHandler.h"

namespace Prototype
{
	const int ServerTimeHandler::TICK_DELTA_TIME = static_cast<int>(1000 / TICKS_PER_SECOND);

	int ServerTimeHandler::getTickWithTimeout()
	{
		return (getTime() - tickStartTime - WAIT_FOR_TICK_TIMEOUT) / TICKS_PER_SECOND;
	}

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


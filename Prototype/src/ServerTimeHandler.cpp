#include "ServerTimeHandler.h"

namespace Prototype
{

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


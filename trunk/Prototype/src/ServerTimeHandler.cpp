#include "ServerTimeHandler.h"

namespace Prototype
{
	int ServerTimeHandler::getTickFromTimeWithTimeout()
	{
		int tick = calculateTickFromTime(getTime());
		int tickWithTimeout = calculateTickFromTime(getTime() - WAIT_FOR_TICK_TIMEOUT);

		if (tick == tickWithTimeout)
			tickWithTimeout--;

		return tickWithTimeout;
	}

	int ServerTimeHandler::getTickFromTime()
	{
		return calculateTickFromTime(getTime());
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

	int ServerTimeHandler::calculateTickFromTime(int time)
	{
		return static_cast<int>(static_cast<float>(time) / static_cast<float>(TICK_DELTA_TIME));
	}
};


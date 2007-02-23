#include "ServerTimeHandler.h"

namespace Prototype
{

	int ServerTimeHandler::getTickFromTimeWithTimeout()
	{
		return calculateTickFromTime(getTime() - WAIT_FOR_TICK_TIMEOUT);
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
		return time / TICK_DELTA_TIME;
	}

};


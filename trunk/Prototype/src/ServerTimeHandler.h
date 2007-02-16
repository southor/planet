#ifndef __servertimehandler_h__
#define __servertimehandler_h__

#include "SDL.h"

#include "TimeHandler.h"

namespace Prototype
{
	class ServerTimeHandler : public TimeHandler
	{
	public:
		static const int WAIT_FOR_TICK_TIMEOUT = 10;
		static const int TICKS_PER_SECOND = 50;
		static const int TICK_DELTA_TIME;

		ServerTimeHandler() : tick(0) {}

		int getTick()		{ return tick; }

		void nextTick();

		void reset();

	private:
		int tick;
	};
};
#endif

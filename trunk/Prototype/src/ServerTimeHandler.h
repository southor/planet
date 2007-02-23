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

		ServerTimeHandler() : tick(0) {}

		int getTickFromTimeWithTimeout();

		int getTickFromTime();

		int getTick()						{ return tick; }
	
		void setTick(int tick)				{ this->tick = tick; }

		void nextTick();

		void reset();

	private:
		int calculateTickFromTime(int time);

		int tick;
	};
};
#endif

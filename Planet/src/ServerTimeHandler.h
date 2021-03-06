#ifndef __servertimehandler_h__
#define __servertimehandler_h__

#include "SDL.h"

#include "TimeHandler.h"

namespace Planet
{
	class ServerTimeHandler : public TimeHandler
	{
	public:
		static const int WAIT_FOR_TICK_TIMEOUT = 50;

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

	// used to set size of history list
	static const size_t SERVER_N_HISTORY_TICKS = static_cast<size_t>((MAX_CLIENT_TO_SERVER_LAG + MAX_SERVER_TO_CLIENT_LAG) / TimeHandler::TICK_DELTA_TIME);
};
#endif

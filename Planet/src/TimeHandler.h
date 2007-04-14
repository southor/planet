#ifndef __timehandler_h__
#define __timehandler_h__

#include "SDL.h"

namespace Planet
{
	class TimeHandler
	{
	public:

		static const int TICKS_PER_SECOND = 20;
		static const int TICK_DELTA_TIME = 1000 / TICKS_PER_SECOND;

		TimeHandler() : startTime(0)
		{}

		inline int getTime()				{ return SDL_GetTicks() - startTime; }
		
		inline void incrementTime(int inc)	{ startTime -= inc; }
		
		virtual void reset();

	private:
		int startTime;
	};
	
	static const int MAX_SERVER_TO_CLIENT_LAG = 1500;
	static const int MAX_CLIENT_TO_SERVER_LAG = 1500;

};
#endif

#ifndef __servertimehandler_h__
#define __servertimehandler_h__

#include "SDL.h"

namespace Prototype
{
	class ServerTimeHandler
	{
	public:

		static const int DELTA_TIME_MAX = 1000/30;

		static const float DELTA_TIME_MAX_F;
		
		static const int TICKS_PER_SECOND = 50;

		ServerTimeHandler() : startTime(0), stepTime(0), deltaTime(1)
		{}

		inline int getTime()				{ return SDL_GetTicks() - startTime; }

		int getStepTime()					{ return stepTime; }

		int getDeltaTime()					{ return deltaTime; }

		float getDeltaTimef()				{ return static_cast<float>(deltaTime); }
		
		int getTick();
		
		void reset();

		void nextStep();
		
	private:
		int startTime;
		int stepTime;
		int deltaTime;
	};
};
#endif

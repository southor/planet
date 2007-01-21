#ifndef __timehandler_h__
#define __timehandler_h__

#include "SDL.h"

namespace Prototype
{
	class TimeHandler
	{
	public:

		static const int DELTA_TIME_MAX = 1000/30;

		static const float DELTA_TIME_MAX_F;

		TimeHandler() : startTime(0), stepTime(0), deltaTime(1)
		{}

		inline int getTime()				{ return SDL_GetTicks() - startTime; }

		int getStepTime()					{ return stepTime; }

		int getDeltaTime()					{ return deltaTime; }

		float getDeltaTimef()				{ return static_cast<float>(deltaTime); }
		
		void reset();

		void nextStep();
		
	private:
		int startTime;
		int stepTime;
		int deltaTime;
	};
};
#endif
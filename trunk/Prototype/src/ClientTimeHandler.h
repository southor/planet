#ifndef __clienttimehandler_h__
#define __clienttimehandler_h__

#include "TimeHandler.h"

namespace Prototype
{
	class ClientTimeHandler : public TimeHandler
	{
	public:

		static const int DELTA_TIME_MAX = 1000/30;

		static const float DELTA_TIME_MAX_F;
		
		static const int TICKS_PER_SECOND = 50;

		ClientTimeHandler() : TimeHandler()
		{}

		//inline int getTime()				{ return SDL_GetTicks() - startTime; }

		int getStepTime()					{ return stepTime; }

		int getDeltaTime()					{ return deltaTime; }

		float getDeltaTimef()				{ return static_cast<float>(deltaTime); }
		
		int getTick();
		
		void reset();

		void nextStep();
		
	private:
		//int startTime;
		int stepTime;
		int deltaTime;
	};
};
#endif

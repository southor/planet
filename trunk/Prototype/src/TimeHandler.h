#ifndef __timehandler_h__
#define __timehandler_h__

namespace Prototype
{
	class TimeHandler
	{
	public:
		TimeHandler() : startTime(0), stepTime(0), deltaTime(1)
		{}

		inline int getTime()				{ return SDL_GetTicks() - startTime; }

		int getStepTime()					{ return stepTime; }

		int getDeltaTime()					{ return deltaTime; }

		float getDeltaTimef()				{ return static_cast<float>(deltaTime); }
		
		void reset() 
		{ 
			startTime = SDL_GetTicks();
			stepTime = 0;
			deltaTime = 1;
		}

		void nextStep()
		{
			int preStepTime = stepTime;
			stepTime = getTime();
			deltaTime = stepTime - preStepTime;
			if (deltaTime == 0) deltaTime = 1; // avoiding division with zero problems
		}
		
	private:
		int startTime;
		int stepTime;
		int deltaTime;
	};
};
#endif
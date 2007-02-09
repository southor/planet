#include "TimeHandler.h"

namespace Prototype
{
	const float TimeHandler::DELTA_TIME_MAX_F = static_cast<float>(DELTA_TIME_MAX);
		
	void TimeHandler::reset() 
	{ 
		startTime = SDL_GetTicks();
		stepTime = 0;
		deltaTime = 1;
	}

	void TimeHandler::nextStep()
	{
		int preStepTime = stepTime;
		stepTime = getTime();
		deltaTime = stepTime - preStepTime;
		if (deltaTime == 0) deltaTime = 1; // avoiding division with zero problems
	}
};


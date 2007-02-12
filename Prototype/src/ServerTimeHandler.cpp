#include "ServerTimeHandler.h"

namespace Prototype
{
	const float ServerTimeHandler::DELTA_TIME_MAX_F = static_cast<float>(DELTA_TIME_MAX);

	int ServerTimeHandler::getTick()
	{
		return getStepTime() / TICKS_PER_SECOND;
	}
		
	void ServerTimeHandler::reset() 
	{ 
		startTime = SDL_GetTicks();
		stepTime = 0;
		deltaTime = 1;
	}

	void ServerTimeHandler::nextStep()
	{
		int preStepTime = stepTime;
		stepTime = getTime();
		deltaTime = stepTime - preStepTime;
		if (deltaTime == 0) deltaTime = 1; // avoiding division with zero problems
	}
};


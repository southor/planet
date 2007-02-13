#include "ClientTimeHandler.h"

namespace Prototype
{
	const float ClientTimeHandler::DELTA_TIME_MAX_F = static_cast<float>(DELTA_TIME_MAX);

	int ClientTimeHandler::getTick()
	{
		return getStepTime() / TICKS_PER_SECOND;
	}
		
	void ClientTimeHandler::reset() 
	{ 
		TimeHandler::reset();
		deltaTime = 1;
	}

	void ClientTimeHandler::nextStep()
	{
		int preStepTime = stepTime;
		stepTime = getTime();
		deltaTime = stepTime - preStepTime;
		if (deltaTime == 0) deltaTime = 1; // avoiding division with zero problems
	}
};


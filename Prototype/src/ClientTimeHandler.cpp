#include "ClientTimeHandler.h"

namespace Prototype
{
	const float ClientTimeHandler::DELTA_TIME_MAX_F = static_cast<float>(DELTA_TIME_MAX);

	//int ClientTimeHandler::getTick()
	//{
	//	return getStepTime() / TICKS_PER_SECOND;
	//}

	double ClientTimeHandler::getStepTick()
	{
		return static_cast<double>(getStepTime() - tick0Time)
				/ static_cast<double>(TICK_DELTA_TIME);
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


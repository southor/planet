#include "ClientTimeHandler.h"

namespace Prototype
{
	const float ClientTimeHandler::DELTA_TIME_MAX_F = static_cast<float>(DELTA_TIME_MAX);

	//int ClientTimeHandler::getTick()
	//{
	//	return getStepTime() / TICKS_PER_SECOND;
	//}

	//Tickf ClientTimeHandler::getStepTick()
	//{
	//	return static_cast<Tickf>(getStepTime() - tick0Time)
	//			/ static_cast<Tickf>(TICK_DELTA_TIME);
	//}

	// returns true only once for every new Tick
	bool ClientTimeHandler::isNewTick()
	{
		bool tmp = newTick;
		newTick = false;
		return tmp;
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

		int nextStepTick = static_cast<Tickf>(stepTime - tick0Time)
					/ static_cast<Tickf>(TICK_DELTA_TIME);

		if (stepTick != nextStepTick)
		{
			newTick = true;
			stepTick = nextStepTick;
		}
	}
};


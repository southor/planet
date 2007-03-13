#include "ClientTimeHandler.h"

namespace Prototype
{
	//const float ClientTimeHandler::DELTA_TIME_MAX_F = static_cast<float>(DELTA_TIME_MAX);

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
		
	//void ClientTimeHandler::reset() 
	//{ 
	//	TimeHandler::reset();
	//	deltaTime = 1;
	//}

	void ClientTimeHandler::nextStep()
	{
		//int preStepTime = stepTime;
		//stepTime = getTime();
		//deltaTime = stepTime - preStepTime;
		//if (deltaTime == 0) deltaTime = 1; // avoiding division with zero problems

		int preStepTick = stepTick;

		stepTickf = tmax(stepTickf, static_cast<Tickf>(getTime() - getTick0Time()) / static_cast<Tickf>(TICK_DELTA_TIME));
		stepTick = static_cast<int>(stepTickf);

		if (stepTick != preStepTick)
		{
			assert(stepTick > preStepTick);
			stepTick = preStepTick + 1; // only step one tick at a time
			newTick = true;
		}
	}

	int ClientTimeHandler::getTickTime()
	{
		return getTick0Time() + stepTick * TICK_DELTA_TIME;
	}

	void ClientTimeHandler::setTick0Time(int tick0Time)
	{
		for(int i=0; i<TICK0TIME_BUFFER_SIZE; ++i)
		{
			tick0TimeBuffer[i] = tick0Time;
		}
		tick0TimeBufferSum = tick0Time * TICK0TIME_BUFFER_SIZE;
		currTimeBufferPos = 0;
	}

	void ClientTimeHandler::enterTick0Time(int tick0Time)
	{
		tick0TimeBufferSum -= tick0TimeBuffer[currTimeBufferPos];		
		tick0TimeBuffer[currTimeBufferPos] = tick0Time;
		tick0TimeBufferSum += tick0Time;

		currTimeBufferPos = (currTimeBufferPos + 1) % TICK0TIME_BUFFER_SIZE;
	}

};


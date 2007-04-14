#include "ClientTimeHandler.h"

namespace Prototype
{
	// returns true only once for every new Tick
	bool ClientTimeHandler::isNewTick()
	{
		bool tmp = newTick;
		newTick = false;
		return tmp;
	}
		
	void ClientTimeHandler::nextStep()
	{
		Tickf stepTickfTmp = tmax(stepTickf, static_cast<Tickf>(getTime() - getTick0Time()) / static_cast<Tickf>(TICK_DELTA_TIME));
		int stepTickTmp = static_cast<int>(stepTickfTmp);

		if (stepTickTmp >= stepTick)
		{
			int preStepTick = stepTick;
			stepTickf = stepTickfTmp;
			stepTick = stepTickTmp;
			
			if (stepTick > preStepTick)
			{
				assert(stepTick > preStepTick);
				if (stepTick > preStepTick + 1)
				{
					stepTick = preStepTick + 1; // only step one tick at a time
					stepTickf = static_cast<Tickf>(stepTick);
				}

				newTick = true;				
			}

			//assert(((stepTick > preStepTick) || !newTick) && (stepTick >= preStepTick));
		}

		//assert(((stepTick > preStepTick) || !newTick) && (stepTick >= preStepTick));
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


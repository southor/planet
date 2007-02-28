#include "MessageReciever.h"
#include "common.h"



namespace Prototype
{
	bool MessageReciever::hasMessageOnQueue()
	{
		if (lagQueue.size() > 0)
		{
			return lagTimeHandler.getTime() >= (lagQueue.front().time + simulatedLag);
			
			/*
			// USED FOR DEBUGGING, DOES THE SAME AS THE CODE ABOVE
			int t1 = lagTimeHandler.getTime();
			int t2 = lagQueue.front().time;

			bool state =  t1 >= t2 + simulatedLag;

			return state;
			*/
		}
		return false;
	}

	bool MessageReciever::hasMessageOnQueueWithTick(int tick)
	{
		if (hasMessageOnQueue())
		{
			return lagQueue.front().tick <= tick;
		}
		
		return false;		
	}

	int MessageReciever::getTickOfMessageOnQueue()
	{
		if (hasMessageOnQueue())
		{
			return lagQueue.front().tick;
		}
		else
		{
			assert(false);
		}
	}

	void MessageReciever::putMessageToLagQueue(Message message, int currentTime)
	{		
		lag = (currentTime - message.time) + simulatedLag;
		//LagMessage lagMessage(message, lagTimeHandler.getTime());
		message.time = lagTimeHandler.getTime();
		lagQueue.push_back(message);
		
		if (message.tick > latestTick)
			latestTick = message.tick;
	}
	
	Message MessageReciever::popMessage()
	{
		assert(hasMessageOnQueue());
		Message message(lagQueue.front());
		lagQueue.pop_front();
		return message;
	}
};


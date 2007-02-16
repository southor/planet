#include "MessageReciever.h"
#include "common.h"



namespace Prototype
{
	bool MessageReciever::hasMessageOnQueue()
	{
		if (lagQueue.size() > 0)
		{
			return lagTimeHandler.getTime() >= (lagQueue.front().time + simulatedLag);
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
			assert(true);
			return 0;
		}
	}

	void MessageReciever::putMessageToLagQueue(Message message, int currentTime)
	{		
		ping = (currentTime - message.time) + simulatedLag;
		//LagMessage lagMessage(message, lagTimeHandler.getTime());
		message.time = lagTimeHandler.getTime();
		lagQueue.push_back(message);
	}
	
	Message MessageReciever::popMessage()
	{
		assert(hasMessageOnQueue());
		Message message(lagQueue.front());
		lagQueue.pop_front();
		return message;
	}
};


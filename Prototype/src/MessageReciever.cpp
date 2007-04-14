#include "MessageReciever.h"
#include "common.h"



namespace Prototype
{
	bool MessageReciever::hasMessageOnQueue()
	{
		if (recieveQueue.size() > 0)
		{
			return true;
			//return lagTimeHandler.getTime() >= (lagQueue.front().time + simulatedLag);
		}
		return false;
	}

	bool MessageReciever::hasMessageOnQueueWithTick(int tick)
	{
		if (hasMessageOnQueue())
		{
			return recieveQueue.front().tick <= tick;
		}
		
		return false;		
	}

	int MessageReciever::getTickOfMessageOnQueue()
	{
		if (hasMessageOnQueue())
		{
			return recieveQueue.front().tick;
		}
		else
		{
			assert(false);
			return 0;
		}
	}
	
	void MessageReciever::retrieve(int currentTime)
	{
		while (lagQueue.size() > 0 && (lagTimeHandler.getTime() >= (lagQueue.front().time + simulatedLag)))
		{
			Message message = lagQueue.front();
			lagQueue.pop_front();

			recieveQueue.push_back(message);

			if (message.tick > getLatestTick())
				setLatestTick(message.tick);
		}
	}

	void MessageReciever::putMessageToLagQueue(Message message, int currentTime)
	{
		lag = (currentTime - message.time) + simulatedLag;
		message.time = lagTimeHandler.getTime();
		lagQueue.push_back(message);
	}
	
	Message MessageReciever::popMessage()
	{
		assert(hasMessageOnQueue());
		Message message(recieveQueue.front());
		recieveQueue.pop_front();
		return message;
	}
};


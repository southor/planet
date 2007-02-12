#include "MessageReciever.h"
#include "common.h"



namespace Prototype
{
	bool MessageReciever::hasMessageOnQueue()
	{
		if (lagQueue.size() > 0)
		{
			return timeHandler.getTime() >= (lagQueue.front().time + lag);
		}
		return false;
	}

	bool MessageReciever::hasMessageOnQueueWithTick(int tick)
	{
		if (hasMessageOnQueue())
		{
			return lagQueue.front().message.tick <= tick;
		}
		
		return false;		
	}

	int MessageReciever::getTickOfMessageOnQueue()
	{
		if (hasMessageOnQueue())
		{
			return lagQueue.front().message.tick;
		}
		else
		{
			assert(true);
		}
	}

	void MessageReciever::putMessageToLagQueue(const Message &message)
	{
		LagMessage lagMessage(message, timeHandler.getTime());
		lagQueue.push_back(lagMessage);
	}
	
	Message MessageReciever::popMessage()
	{
		assert(hasMessageOnQueue());
		Message message(lagQueue.front().message);
		lagQueue.pop_front();
		return message;
	}
};


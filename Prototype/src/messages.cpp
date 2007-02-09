#include "messages.h"
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


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
		/*
		// ------------------ DEBUG PURPOSE --------------------
		if (lagQueue.size() > 0 && (lagTimeHandler.getTime() >= (lagQueue.front().time + 10)))
		{
			if (lagQueue.size() != 0)
			{
				printf("%d YES lagQueue.size(): %d, lagTimeHanlder.getTime(): %d, lagQueue.front().time: %d\n", 
					v, lagQueue.size(), lagTimeHandler.getTime(), lagQueue.front().time);
			}
			else
			{
				printf("%d YES lagQueue.size(): %d\n", 
					v, lagQueue.size());
			}
		}
		else
		{
			if (lagQueue.size() != 0)
			{
				printf("%d NO lagQueue.size(): %d, lagTimeHanlder.getTime(): %d, lagQueue.front().time: %d\n", 
					v, lagQueue.size(), lagTimeHandler.getTime(), lagQueue.front().time);
			}
			else
			{
				printf("%d NO lagQueue.size(): %d\n", 
					v, lagQueue.size());
			}
		}
		*/

		while (lagQueue.size() > 0 && (lagTimeHandler.getTime() >= (lagQueue.front().time + simulatedLag)))
		{
			Message message = lagQueue.front();
			lagQueue.pop_front();
		
			//printf("%d pop %d from lagqueue and push to recievequeue\n", v, message.tick);
					
			recieveQueue.push_back(message);

			if (message.tick > getLatestTick())
				setLatestTick(message.tick);
		}
	}

	void MessageReciever::putMessageToLagQueue(Message message, int currentTime)
	{
		lag = (currentTime - message.time) + simulatedLag;
		//LagMessage lagMessage(message, lagTimeHandler.getTime());
		message.time = lagTimeHandler.getTime();
		lagQueue.push_back(message);

		//printf("%d putting %d on lag queue\n", v, message.tick);
		
		//MessageReciever::retrieve(0);
		
		/*
		while (lagQueue.size() > 0)
		{
			Message m = lagQueue.front();
			lagQueue.pop_front();
			
			recieveQueue.push_back(m);

			if (m.tick > getLatestTick())
				setLatestTick(m.tick);
		}
		*/

	}
	
	Message MessageReciever::popMessage()
	{
		assert(hasMessageOnQueue());
		Message message(recieveQueue.front());
		recieveQueue.pop_front();
		return message;
	}
};


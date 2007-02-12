#ifndef __messagereciever_h__
#define __messagereciever_h__

//#include <vector>

#include "Message.h"
#include "TimeHandler.h"
#include "basic.h"

#include <deque>


namespace Prototype
{
	class MessageReciever
	{
	private:
		struct LagMessage
		{
			Message message;
			int time;
			
			LagMessage(const Message &message, int time)
				: message(message), time(time)
			{}
		};

		int lag;
		std::deque<LagMessage> lagQueue;
		TimeHandler timeHandler;

	public:
		MessageReciever() : lag(0)		{}
		virtual ~MessageReciever()		{}
		
		virtual bool hasMessageOnQueue();
		//virtual bool hasMessageOnQueueWithCurrentTick();
		virtual bool hasMessageOnQueueWithTick(int tick);
		

		void setSimulatedLag(int lag)			{ assert(lag >= 0); this->lag = lag; }
		
		void putMessageToLagQueue(const Message &message);
		Message popMessage();
	};
};

#endif

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

		int ping;
		int simulatedLag;
		std::deque<Message> lagQueue;
		TimeHandler lagTimeHandler;

	public:
		MessageReciever() : simulatedLag(0), ping(0)	{}
		virtual ~MessageReciever()						{}
		
		virtual bool hasMessageOnQueue();
		virtual bool hasMessageOnQueueWithTick(int tick);
		virtual int getTickOfMessageOnQueue();
		inline int getCurrentPing()					{ return ping; }
		
		int getNMessagesOnQueue()							{ return lagQueue.size(); }
		

		void setSimulatedLag(int simulatedLag)		{ assert(simulatedLag >= 0); this->simulatedLag = simulatedLag; }
				
		virtual void retrieve(int currentTime)		{}
		void putMessageToLagQueue(Message message, int currentTime);
		Message popMessage();
	};
};

#endif

#ifndef __messagereciever_h__
#define __messagereciever_h__

//#include <vector>

#include "Message.h"
#include "TimeHandler.h"
#include "basic.h"

#include <deque>
#include <string>

namespace Prototype
{

	class MessageReciever
	{
	private:

		int lag;
		int simulatedLag;
		int lt;
		std::deque<Message> lagQueue;
		std::deque<Message> recieveQueue;
		TimeHandler lagTimeHandler;


	public:
		MessageReciever() : simulatedLag(0), lag(0), lt(0)	{}
		virtual ~MessageReciever()						{}
		
		virtual bool hasMessageOnQueue();
		virtual bool hasMessageOnQueueWithTick(int tick);
		virtual int getTickOfMessageOnQueue();
		inline int getCurrentLag()					{ return lag; }
		int getLatestTick() const					{ return lt; }
		void setLatestTick(int tick)				{ lt = tick; }
		int getNMessagesOnQueue()					{ return static_cast<int>(recieveQueue.size()); }
		

		void setSimulatedLag(int simulatedLag)		{ assert(simulatedLag >= 0); this->simulatedLag = simulatedLag; }
				
		virtual void retrieve(int currentTime);
		void putMessageToLagQueue(Message message, int currentTime);
		Message popMessage();

		int v;

	};
};

#endif

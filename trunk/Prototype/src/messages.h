#ifndef __messages_h__
#define __messages_h__

//#include <vector>

#include "basic.h"
#include "TimeHandler.h"

#include <deque>


namespace Prototype
{
	struct Message
	{
		int type;
		int time;
		int tick;
		int size;
		void *data;
		Message() : type(0), time(0), tick(0), size(0), data(0)			{}
		Message(int type, int size, void *data, int time = 0, int tick = 0)
			: type(type), size(size), data(data), time(time), tick(tick)	{}
	};

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
		virtual bool hasMessageOnQueueWithCurrentTick();
		virtual bool hasMessageOnQueueWithTick(int tick);
		

		void setSimulatedLag(int lag)			{ assert(lag >= 0); this->lag = lag; }
		
		void putMessageToLagQueue(const Message &message);
		Message popMessage();
	};

	class MessageSender
	{
	public:
		MessageSender()					{}
		virtual ~MessageSender()		{}
		
		virtual void pushMessage(const Message &message) = 0;

		// Get number of messages queued to be transmitted.
		virtual int getNMessages() = 0;

		// Transmits queued messages.
		virtual void transmit() = 0;
	};
};

#endif

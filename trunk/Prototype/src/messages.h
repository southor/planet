#ifndef __messages_h__
#define __messages_h__

//#include <vector>

#include "basic.h"
#include "TimeHandler.h"

#include <deque>


namespace Prototype
{
	//class Message
	//{
	//private:
	//	int type;
	//	void *data;

	//	Message& operator =(const Message &message);
	//public:
	//	Message() : type(0), data(NULL)			{}
	//	~Message()								{ if (data != NULL) delete data; }

	//	// @param data This Message will take over the ownership of the data.
	//	void set(int type, void *data)			{ this->type = type;
	//											  if (this->data != NULL) delete data;
	//											  this->data = data; }

	//	int getType() const						{ return type; }
	//	void* getData()	const					{ return data; }		
	//};

	struct Message
	{
		int type;
		int time;
		void *data;
		Message() : type(0),  data(0), time(0)		{}
		Message(int type, void *data, int time = 0)
			: type(type), data(data), time(time)	{}
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
		
		//// Get number of messages recieved.
		//int getNMessages();

		bool hasMessageOnQueue();

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
#ifndef __Link_h__
#define __Link_h__

#include "messages.h"
#include "MessageReciever.h"
#include "MessageSender.h"

namespace Prototype
{




	class Link
	{
	private:
		MessageSender *messageSender;
		MessageReciever *messageReciever;
		
		Message poppedMessage;
		bool hasPoppedMessage;

		//bool hasPoppedMessage()		{ poppedMessage.data != NULL; }
		//void resetPoppedMessage()		{ hasPoppedMessage = false; }
		void destroyPoppedMessage();

		// pushes a Message to the send queue, also sets time of message
		void pushMessage(int type, int size, void *data) const;

		// returns the data member of the popped message
		void* getPoppedData() const;

	public:
	
		inline void setMessageSender(MessageSender *messageSender)
		{
			assert(messageSender);
			this->messageSender = messageSender;
		}
		
		void setMessageReciever(MessageReciever *messageReciever)
		{
			assert(messageReciever);
			this->messageReciever = messageReciever;
		}

		Link() : messageSender(0), messageReciever(0)
		{
			hasPoppedMessage = false;
		}

		Link(MessageSender *messageSender, MessageReciever *messageReciever) 
			: messageSender(messageSender), messageReciever(messageReciever)
		{
			//resetPoppedMessage();
			hasPoppedMessage = false;
		}

		~Link()
		{
			destroyPoppedMessage();
		}

		// --------------------------------- sending messages ------------------------------

		void transmit() const; // transmits all messages to the send queue

		template <class Cmd>
		void pushMessage(const Cmd &cmd) const
		{
			pushMessage(Cmd::messageType, sizeof(Cmd), new Cmd(cmd));
		}
		

		// --------------------------------- recieving messages ------------------------------

		//int getNMessages() const; // returns number of recieve messages on the queue
		bool hasMessageOnQueue() const; // returns true if at least 1 message is on recieve queue
		bool hasMessageOnQueueWithTick(int tick) const; // returns true if at least 1 message is on recieve queue		
		int getTickOfMessageOnQueue() const; // returns the tick of the top message on queue
		int popMessage(); // returns the type member of the popped message, (will destroy the last message)	
		int getPoppedType() const; // returns the type member of the popped message		
		
		template <class Cmd>
		Cmd* getPoppedData() const
		{
			return reinterpret_cast<Cmd*>(getPoppedData());
		}
	};
};

#endif

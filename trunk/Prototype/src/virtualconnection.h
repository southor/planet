#ifndef __virtualconnection_h__
#define __virtualconnection_h__

#include <deque>

#include "messages.h"

namespace Prototype
{
	class VirtualConnection
	{
	public:
		
		class VirtualMessageSender : public MessageSender
		{
		public:
			VirtualMessageSender() : MessageSender() { }
			~VirtualMessageSender();
		
			void setMessageDeque(std::deque<Message> *messageDeque);

			void pushMessage(const Message &message);

			// Get number of messages queued to be transmitted.
			int getNMessages();
			
			// Transmits queued messages.
			void transmit();
					
		private:
			std::deque<Message> *messageDeque;
		};

		class VirtualMessageReciever : public MessageReciever
		{
		public:
			VirtualMessageReciever() : MessageReciever() { }
			~VirtualMessageReciever();

			void setMessageDeque(std::deque<Message> *messageDeque);

			// Get number of messages recieved.
			int getNMessages();		
			
			Message popMessage();
		
		private:
			std::deque<Message> *messageDeque;
		};

	public:
		VirtualConnection();

		MessageSender* getMessageSender();
		MessageReciever* getMessageReciever();

	private:
		std::deque<Message> messageDeque;
		
		VirtualMessageSender virtualMessageSender;
		VirtualMessageReciever virtualMessageReciever;
	};
};

#endif
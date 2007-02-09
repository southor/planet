#ifndef __virtualconnection_h__
#define __virtualconnection_h__

#include <deque>

#include "common.h"
#include "messages.h"

namespace Prototype
{
	class VirtualConnection
	{
	public:
		
		class VirtualMessageSender : public MessageSender
		{
		public:
			//VirtualMessageSender() : MessageSender() { }
			VirtualMessageSender(MessageReciever *messageReciever)
				: MessageSender(), messageReciever(messageReciever)
			{}
			
			~VirtualMessageSender()					{}
		
			//void setMessageDeque(std::deque<Message> *messageDeque);

			void pushMessage(const Message &message);

			// Get number of messages queued to be transmitted.
			int getNMessages();
			
			// Transmits queued messages.
			void transmit();
					
		private:
			//std::deque<Message> *messageDeque;
			MessageReciever *messageReciever;
			std::deque<Message> sendDeque;
		};

		//class VirtualMessageReciever : public MessageReciever
		//{
		//public:
		//	VirtualMessageReciever() : MessageReciever(), lag(50) { }
		//	~VirtualMessageReciever() {}

		//	void setMessageDeque(std::deque<Message> *messageDeque);

		//	// Get number of messages recieved.
		//	int getNMessages();		
		//	
		//	Message popMessage();
		//
		//	void retrieve();
		//
		//	void setConnectionLag(int lag);
		//
		//private:
		//	std::deque<Message> *messageDeque;
		//	std::deque<Message> recieveDeque;
		//	
		//	int lag;
		//};

	public:
		VirtualConnection();

		//void setConnectionLag(int lag);
		
		MessageSender* getMessageSender();
		MessageReciever* getMessageReciever();

		//int getNPendingMessages() { return messageDeque.size(); }

	private:
		//std::deque<Message> messageDeque;
		
		VirtualMessageSender virtualMessageSender;
		//VirtualMessageReciever virtualMessageReciever;
		MessageReciever messageReciever;
	};
};

#endif

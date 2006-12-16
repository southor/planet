#ifndef __client_h__
#define __client_h__

#include <iostream>

#include "common.h"
#include "messages.h"

namespace Prototype
{
	class Client
	{
	public:
		Client(MessageSender *messageSender, MessageReciever *messageReciever)
			: messageSender(messageSender), messageReciever(messageReciever) { }

		void logic();
		
		void sendMessage(Message message);
		void recieveMessages();

	private:
		MessageSender *messageSender;
		MessageReciever *messageReciever;
	};
};

#endif

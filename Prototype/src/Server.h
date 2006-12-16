#ifndef __server_h__
#define __server_h__

#include <vector>

#include "messages.h"

namespace Prototype
{
	struct ServerClient
	{
		MessageSender *messageSender;
		MessageReciever *messageReciever;

		ServerClient(MessageSender *messageSender, MessageReciever *messageReciever) 
			: messageSender(messageSender), messageReciever(messageReciever) { }
	};


	class Server
	{
	public:
		Server() { }

		void logic();

		void addClient(MessageSender *messageSender, MessageReciever *messageReciever);

	private:
		void addClient(ServerClient client);

		std::vector<ServerClient> clients;
	};
};

#endif
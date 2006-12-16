#include "Server.h"

namespace Prototype
{

	void Server::addClient(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		ServerClient client(messageSender, messageReciever);
		
		addClient(client);
	}

	void Server::addClient(ServerClient client)
	{
		clients.push_back(client);
	}
	
	void Server::logic()
	{
		// Respond with the same message to all clients
		std::vector<ServerClient>::iterator it;
		for (it = clients.begin(); it != clients.end(); it++)
		{
			ServerClient client = *it;
		
			MessageSender *messageSender = client.messageSender;
			MessageReciever *messageReciever = client.messageReciever;
			
			while (messageReciever->getNMessages() > 0)
			{
				Message message = messageReciever->popMessage();
				
				messageSender->pushMessage(message);
			}

			messageSender->transmit();
		}		
	}
	
};

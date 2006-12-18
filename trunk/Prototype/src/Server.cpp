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
		// Read messages from clients
		std::vector<ServerClient>::iterator it;
		for (it = clients.begin(); it != clients.end(); it++)
		{
			ServerClient client = *it;

			MessageSender *messageSender = client.messageSender;
			MessageReciever *messageReciever = client.messageReciever;
			


			while (messageReciever->getNMessages() > 0)
			{
				Message message = messageReciever->popMessage();

				if (message.type == USER_CMD)
				{
					UserCmd *userCmd = (UserCmd*)message.data;

					// TODO
					
					printf("userCmd.cmd_left = %d\n", userCmd->cmd_left);
				}			
				delete message.data;
			}

		}		

		// Send updates to clients
		for (it = clients.begin(); it != clients.end(); it++)
		{
			ServerClient client = *it;

			MessageSender *messageSender = client.messageSender;
			MessageReciever *messageReciever = client.messageReciever;


			// TODO
			
			messageSender->transmit();
		}
	}
	
};

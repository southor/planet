#include "Client.h"

namespace Prototype
{

	void Client::logic()
	{
	}

	void Client::sendMessage(Message message)
	{
		std::cout << "sending message, type: " << message.type << ", data: " << *((std::string*)message.data) << std::endl;

		messageSender->pushMessage(message);
	}
	
	void Client::recieveMessages()
	{
		while (messageReciever->getNMessages() != 0)
		{
			Message message = messageReciever->popMessage();
		
			std::cout << "message recieved, type: " << message.type << ", data: " << *((std::string*)message.data) << std::endl;
		}
	}


};

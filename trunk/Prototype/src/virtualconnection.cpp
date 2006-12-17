#include "virtualconnection.h"
#include "assert.h"

namespace Prototype
{
	// --------------------------------------------------------------------------------------
	// ----------------------------------   VirtualConnection  ------------------------------
	// --------------------------------------------------------------------------------------

	VirtualConnection::VirtualConnection()
	{
		virtualMessageSender.setMessageDeque(&messageDeque);
		virtualMessageReciever.setMessageDeque(&messageDeque);
	}

	MessageSender* VirtualConnection::getMessageSender()
	{
		return &virtualMessageSender;
	}

	MessageReciever* VirtualConnection::getMessageReciever()
	{
		return &virtualMessageReciever;
	}

	void VirtualConnection::setConnectionLag(int lag)
	{
		virtualMessageReciever.setConnectionLag(lag);
	}


	// --------------------------------------------------------------------------------------
	// ----------------------------------   VirtualMessageSender  ---------------------------
	// --------------------------------------------------------------------------------------

	void VirtualConnection::VirtualMessageSender::setMessageDeque(std::deque<Message> *messageDeque)
	{
		this->messageDeque = messageDeque;
	}
	
	void VirtualConnection::VirtualMessageSender::pushMessage(const Message &message)
	{
		sendDeque.push_front(message);
	}

	int VirtualConnection::VirtualMessageSender::getNMessages()
	{
		return sendDeque.size();
	}

	void VirtualConnection::VirtualMessageSender::transmit()
	{
		while (!sendDeque.empty())
		{
			Message message = sendDeque.back();
			sendDeque.pop_back();
		
			messageDeque->push_front(message);
		}

	}
	

	// --------------------------------------------------------------------------------------
	// ----------------------------------   VirtualMessageReciever  -------------------------
	// --------------------------------------------------------------------------------------

	void VirtualConnection::VirtualMessageReciever::setMessageDeque(std::deque<Message> *messageDeque)
	{
		this->messageDeque = messageDeque;
	}

	int VirtualConnection::VirtualMessageReciever::getNMessages()
	{
		retrieve();
	
		return recieveDeque.size();
	}
			
	Message VirtualConnection::VirtualMessageReciever::popMessage()
	{
		//Message message;
		//if (messageDeque->empty())
		//{
		//	//TODO throw exception
		//	assert(false);
		//}
		//else
		//{
		//	message = messageDeque->back();
		//	messageDeque->pop_back();
		//}
		
		Message message = recieveDeque.back();
		recieveDeque.pop_back();
		
		return message;
	}
	
	void VirtualConnection::VirtualMessageReciever::retrieve()
	{
		// Move messages with correct time from messageDeque to recieveDeque
		if (!messageDeque->empty()) 
		{
			Message message = messageDeque->back();

			int currentTime = timeHandler.getTime();

			while (message.time < currentTime - lag)
			{
				messageDeque->pop_back();

				recieveDeque.push_front(message);
				
				if (messageDeque->empty())
				{
					// break while loop
					break;
				}
				
				message = messageDeque->back();
			}
			
		}
	}
	
	void VirtualConnection::VirtualMessageReciever::setConnectionLag(int lag)
	{
		this->lag =  lag;
	}

};




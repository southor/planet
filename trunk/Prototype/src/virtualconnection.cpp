#include "virtualconnection.h"
#include "assert.h"

namespace Prototype
{
	// VirtualConnection
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

	// VirtualMessageSender
	void VirtualConnection::VirtualMessageSender::setMessageDeque(std::deque<Message> *messageDeque)
	{
		this->messageDeque = messageDeque;
	}
	
	void VirtualConnection::VirtualMessageSender::transmit()
	{
	}
	
	void VirtualConnection::VirtualMessageSender::pushMessage(const Message &message)
	{
		messageDeque->push_front(message);
	}

	int VirtualConnection::VirtualMessageSender::getNMessages()
	{
		return messageDeque->size();
	}
	
	// VirtualMessageReciever
	void VirtualConnection::VirtualMessageReciever::setMessageDeque(std::deque<Message> *messageDeque)
	{
		this->messageDeque = messageDeque;
	}

	int VirtualConnection::VirtualMessageReciever::getNMessages()
	{
		return messageDeque->size();
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
		Message message = messageDeque->back();
		messageDeque->pop_back();
		return message;
	}
};
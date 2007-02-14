#include "VirtualConnection.h"
#include "assert.h"

namespace Prototype
{
	// --------------------------------------------------------------------------------------
	// ----------------------------------   VirtualConnection  ------------------------------
	// --------------------------------------------------------------------------------------

	VirtualConnection::VirtualConnection() : virtualMessageSender(&messageReciever)
	{
	}

	MessageSender* VirtualConnection::getMessageSender()
	{
		return &virtualMessageSender;
	}

	MessageReciever* VirtualConnection::getMessageReciever()
	{
		return &messageReciever;
	}

	// --------------------------------------------------------------------------------------
	// ----------------------------------   VirtualMessageSender  ---------------------------
	// --------------------------------------------------------------------------------------

	void VirtualConnection::VirtualMessageSender::pushMessage(const Message &message)
	{
		sendDeque.push_back(message);
	}

	int VirtualConnection::VirtualMessageSender::getNMessages()
	{
		return sendDeque.size();
	}

	void VirtualConnection::VirtualMessageSender::transmit()
	{
		while (!sendDeque.empty())
		{
			Message message = sendDeque.front();
			sendDeque.pop_front();
		
			messageReciever->putMessageToLagQueue(message, message.time);
		}
	}
};





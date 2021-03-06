#include "VirtualConnection.h"
#include "assert.h"

namespace Planet
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
		return static_cast<int>(sendDeque.size());
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

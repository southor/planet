#include "Link.h"
#include "common.h"

namespace Prototype
{
	void Link::destroyPoppedMessage()
	{
		if (hasPoppedMessage)
		{
			delete [] poppedMessage.data;
			//resetPoppedMessage();
			hasPoppedMessage = false;
		}
	}

	// pushes a Message to the send queue, also sets time of message
	void Link::pushMessage(int type, int size, void *data, int currentTime) const
	{
		assert(messageSender);
		
		Message message(type, size, data, currentTime);
		messageSender->pushMessage(message);
	}

	// returns the data member of the popped message
	void* Link::getPoppedData() const
	{
		assert(messageReciever);
		assert(hasPoppedMessage);
		assert(poppedMessage.data);
		
		return poppedMessage.data;
	}

	// ------------------------------------------------------------------------------------
	// ------------------------------------- sending messages -----------------------------
	// ------------------------------------------------------------------------------------

	// transmits all messages to the send queue
	void Link::transmit() const
	{
		assert(messageSender);
		
		messageSender->transmit();
	}

	// ------------------------------------------------------------------------------------
	// ------------------------------------- recieving messages ---------------------------
	// ------------------------------------------------------------------------------------

	// returns true if at least 1 message on recieve queue
	bool Link::hasMessageOnQueue() const
	{
		return messageReciever->hasMessageOnQueue();
	}

	// returns true if at least 1 message on recieve queue
	bool Link::hasMessageOnQueueWithTick(int tick) const
	{
		return messageReciever->hasMessageOnQueueWithTick(tick);
	}

	int Link::getTickOfMessageOnQueue() const
	{
		return messageReciever->getTickOfMessageOnQueue();
	}

	// returns the type member of the popped message, (will destroy the last message)		
	int Link::popMessage()
	{
		assert(messageReciever);

		destroyPoppedMessage();

		assert(hasMessageOnQueue());
		poppedMessage = messageReciever->popMessage();
		hasPoppedMessage = true;

		return poppedMessage.type;
	}

	// returns the type member of the popped message
	int Link::getPoppedType() const
	{
		assert(hasPoppedMessage);
		return poppedMessage.type;
	}
};


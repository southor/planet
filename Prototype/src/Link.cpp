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
	void Link::pushMessage(int type, int size, void *data) const
	{
		assert(messageSender);
		Message message(type, size, data);
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

	//// pushes a UserCmd to the send queue
	//void Link::pushMessage(const UserCmd &userCmd)
	//{
	//	pushMessage(USER_CMD, new UserCmd(userCmd));
	//}

	//// pushes a UpdatePlayerObj to the send queue	
	//void Link::pushMessage(const UpdatePlayerObj &updatePlayerObj)
	//{
	//	pushMessage(UPDATE_PLAYER_OBJ,  new UpdatePlayerObj(updatePlayerObj));
	//}

	//// pushes an AddObstacle to the send queue
	//void pushMessage(const AddObstacle &addObstacle)
	//{
	//}

	// transmits all messages to the send queue
	void Link::transmit() const
	{
		assert(messageSender);
		messageSender->transmit();
	}

	// ------------------------------------------------------------------------------------
	// ------------------------------------- recieving messages ---------------------------
	// ------------------------------------------------------------------------------------

	//// returns number of recieve messages on the queue
	//int Link::getNMessages() const
	//{
	//	assert(messageReciever);
	//	int nmess = messageReciever->getNMessages();
	//	return nmess;
	//}

	// returns if at least 1 message on recieve queue
	bool Link::hasMessageOnQueue() const
	{
		//return getNMessages() >= 1;
		return messageReciever->hasMessageOnQueue();
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


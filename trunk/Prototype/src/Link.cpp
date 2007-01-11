#include "Link.h"
#include "common.h"

namespace Prototype
{

	void Link::destroyPoppedMessage()
	{
		if (hasPoppedMessage)
		{
			delete poppedMessage.data;
			//resetPoppedMessage();
			hasPoppedMessage = false;
		}
	}

	// pushes a Message to the send queue, also sets time of message
	void Link::pushMessage(int type, void *data) const
	{
		assert(messageSender);
		Message message(type, data, timeHandler.getTime());
		messageSender->pushMessage(message);
	}

	// returns the data member of the popped message
	void* Link::getPoppedData() const
	{
		assert(messageReciever);
		assert(hasPoppedMessage);
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

	// returns number of recieve messages on the queue
	int Link::getNMessages() const
	{
		assert(messageReciever);
		return messageReciever->getNMessages();
	}

	// returns if at lesat 1 message on recieve queue
	bool Link::hasMessageOnQueue() const
	{
		return getNMessages() >= 1;
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

	//// returns the data member as a UserCmd
	//UserCmd* Link::getPoppedUserCmd()
	//{
	//	return reinterpret_cast<UserCmd*>(getPoppedData());
	//}

	//// returns the data member as an UpdatePlayerObj
	//UpdatePlayerObj* Link::getPoppedUpdatePlayerObj()
	//{
	//	return reinterpret_cast<UpdatePlayerObj*>(getPoppedData());
	//}

};

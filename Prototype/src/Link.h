#ifndef __Link_h__
#define __Link_h__

#include "messages.h"

namespace Prototype
{

	enum MessageTypes
	{
		UPDATE_PLAYER_OBJ,
		
		USER_CMD
	};

	struct UpdatePlayerObj
	{
		int playerObjId;
		Pos pos;
		float angle;

		UpdatePlayerObj()		{}
		UpdatePlayerObj(int playerObjId, const Pos &pos, float angle)
			: playerObjId(playerObjId), pos(pos), angle(angle)
		{}
	};

	struct UserCmd
	{
		bool cmdLeft;
		bool cmdRight;
		bool cmdUp;
		bool cmdDown;
		bool cmdShoot;
	};




	class Link
	{
	private:
		MessageSender *messageSender;
		MessageReciever *messageReciever;
		
		Message poppedMessage;
		bool hasPoppedMessage;

		//bool hasPoppedMessage()		{ poppedMessage.data != NULL; }
		//void resetPoppedMessage()		{ hasPoppedMessage = false; }
		void destroyPoppedMessage();

		// pushes a Message to the send queue, also sets time of message
		void pushMessage(int type, void *data);

		// returns the data member of the popped message
		void* getPoppedData();

	public:
		
		inline void setMessageSender(MessageSender *messageSender)
		{
			assert(messageSender);
			this->messageSender = messageSender;
		}
		
		void setMessageReciever(MessageReciever *messageReciever)
		{
			assert(messageReciever);
			this->messageReciever = messageReciever;
		}

		Link() : messageSender(0), messageReciever(0)
		{}

		Link(MessageSender *messageSender, MessageReciever *messageReciever) 
			: messageSender(messageSender), messageReciever(messageReciever)
		{
			//resetPoppedMessage();
			hasPoppedMessage = false;
		}

		~Link()
		{
			destroyPoppedMessage();
		}

		// sending messages
		void pushMessage(const UserCmd &userCmd); // pushes a UserCmd to the send queue
		void pushMessage(const UpdatePlayerObj &updatePlayerObj); // pushes a UpdatePlayerObj to the send queue
		void transmit(); // transmits all messages to the send queue

		// recieving messages		
		int getNMessages(); // returns number of recieve messages on the queue
		bool hasMessageOnQueue(); // returns true if at lesat 1 message is on recieve queue
		int popMessage(); // returns the type member of the popped message, (will destroy the last message)	
		int getPoppedType(); // returns the type member of the popped message		
		UserCmd* getPoppedUserCmd(); // returns the data member as a UserCmd
		UpdatePlayerObj* getPoppedUpdatePlayerObj(); // returns the data member as an UpdatePlayerObj


	};



};

#endif
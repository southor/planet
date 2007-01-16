#ifndef __Link_h__
#define __Link_h__

#include "messages.h"
#include "Rectangle.h"
#include "Color.h"

namespace Prototype
{

	enum MessageTypes
	{
		UPDATE_PLAYER_OBJ,		
		USER_CMD,
		ADD_OBSTACLE,
		WELCOME_CLIENT,
		ADD_PLAYER,
		INIT_CLIENT	
	};

	// --------------------------------------------------------------------------------
	// --------------------------------- server messages ------------------------------
	// --------------------------------------------------------------------------------

	struct UpdatePlayerObj
	{
		//size_t playerObjId;
		size_t playerId;
		Pos pos;
		float angle;

		UpdatePlayerObj()		{}
		//UpdatePlayerObj(size_t playerObjId, const Pos &pos, float angle)
		//	: playerObjId(playerObjId), pos(pos), angle(angle)
		UpdatePlayerObj(size_t playerId, const Pos &pos, float angle)
			: playerId(playerId), pos(pos), angle(angle)
		{}
	};

	struct AddObstacle
	{
		size_t obstacleId;
		Rectangle obstacleArea;
		AddObstacle(size_t obstacleId, const Rectangle &obstacleArea)
			: obstacleId(obstacleId), obstacleArea(obstacleArea)
		{}
	};

	struct WelcomeClient
	{
		size_t playerId;

		WelcomeClient(size_t playerId) 
			: playerId(playerId) {}
	};

	struct AddPlayer
	{
		size_t playerId;
		Color color;
		Pos startPos;
		
		AddPlayer(size_t playerId, Color color, Pos startPos) 
			: playerId(playerId), color(color), startPos(startPos) {}
	};


	// --------------------------------------------------------------------------------
	// --------------------------------- client messages ------------------------------
	// --------------------------------------------------------------------------------

	struct InitClient
	{
		Color color;
		
		InitClient(Color color) : color(color) {}
	};

	struct UserCmd
	{
		bool cmdLeft;
		bool cmdRight;
		bool cmdUp;
		bool cmdDown;
		bool cmdShoot;
		float viewangle;
	};





	class Link
	{
	public:
		MessageSender *messageSender;
		MessageReciever *messageReciever;
		
		Message poppedMessage;
		bool hasPoppedMessage;

		//bool hasPoppedMessage()		{ poppedMessage.data != NULL; }
		//void resetPoppedMessage()		{ hasPoppedMessage = false; }
		void destroyPoppedMessage();

		// pushes a Message to the send queue, also sets time of message
		void pushMessage(int type, void *data) const;

		// returns the data member of the popped message
		void* getPoppedData() const;

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
		{
			hasPoppedMessage = false;
		}

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

		// --------------------------------- sending messages ------------------------------

		void transmit() const; // transmits all messages to the send queue

		void pushMessage(const UserCmd &userCmd) const					{ pushMessage(USER_CMD, new UserCmd(userCmd)); }
		void pushMessage(const UpdatePlayerObj &updatePlayerObj) const	{ pushMessage(UPDATE_PLAYER_OBJ,  new UpdatePlayerObj(updatePlayerObj)); }
		void pushMessage(const AddObstacle &addObstacle) const			{ pushMessage(ADD_OBSTACLE,  new AddObstacle(addObstacle)); }
		void pushMessage(const InitClient &initClient) const			{ pushMessage(INIT_CLIENT, new InitClient(initClient)); }
		void pushMessage(const WelcomeClient &welcomeClient) const		{ pushMessage(WELCOME_CLIENT, new WelcomeClient(welcomeClient)); }



		// --------------------------------- recieving messages ------------------------------

		int getNMessages() const; // returns number of recieve messages on the queue
		bool hasMessageOnQueue() const; // returns true if at least 1 message is on recieve queue
		int popMessage(); // returns the type member of the popped message, (will destroy the last message)	
		int getPoppedType() const; // returns the type member of the popped message		
		
		
		UserCmd* getPoppedUserCmd()	const						{ return reinterpret_cast<UserCmd*>(getPoppedData()); }
		UpdatePlayerObj* getPoppedUpdatePlayerObj() const		{ return reinterpret_cast<UpdatePlayerObj*>(getPoppedData()); }
		AddObstacle* getPoppedAddObstacle() const				{ return reinterpret_cast<AddObstacle*>(getPoppedData()); }
		InitClient* getPoppedInitClient() const					{ return reinterpret_cast<InitClient*>(getPoppedData()); }
		WelcomeClient* getPoppedWelcomeClient() const			{ return reinterpret_cast<WelcomeClient*>(getPoppedData()); }

	};



};

#endif
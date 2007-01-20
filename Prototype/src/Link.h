#ifndef __Link_h__
#define __Link_h__

#include "messages.h"
#include "Rectangle.h"
#include "Color.h"

namespace Prototype
{

	enum MessageTypes
	{
		UPDATE_PLAYER_OBJ = 0, // ?
		ADD_PLAYER_OBJ,		
		ADD_OBSTACLE,
		WELCOME_CLIENT,
		ADD_PLAYER,
		INIT_CLIENT,
		ADD_PROJECTILE,
		UPDATE_PROJECTILE,
		REMOVE_PROJECTILE,
		KILL,
		USER_CMD,
		SHOOT_CMD,
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
		UpdatePlayerObj(size_t playerId, const Pos &pos, float angle)
			: playerId(playerId), pos(pos), angle(angle)
		{}
	};
	
	struct AddPlayerObj
	{
		Color color;
		Pos pos;
	
		AddPlayerObj(const Color &color, const Pos &pos)
			: color(color), pos(pos) {}
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

	struct AddProjectile
	{
		size_t projectileId;
		int type;
		Pos pos;
		float angle;
		size_t shooterId;

		AddProjectile(size_t projectileId, int type, Pos pos, float angle, size_t shooterId)
			: projectileId(projectileId), type(type), pos(pos), angle(angle), shooterId(shooterId)
		{}
	};

	struct UpdateProjectile
	{
		size_t projectileId;
		Pos pos;

		UpdateProjectile(size_t projectileId, Pos pos)
			: projectileId(projectileId), pos(pos)
		{}
	};

	struct RemoveProjectile
	{
		size_t projectileId;
		Pos hitPosition;

		RemoveProjectile(size_t projectileId, Pos hitPosition)
			: projectileId(projectileId), hitPosition(hitPosition)
		{}
	};

	struct Kill
	{
		size_t killerId;
		size_t killedId;
		Pos respawnPos;

		Kill(size_t killerId, size_t killedId, Pos respawnPos)
			: killerId(killerId), killedId(killedId), respawnPos(respawnPos)
		{}
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

	struct ShootCmd
	{
		size_t playerId;
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

		void pushMessage(const UserCmd &userCmd) const						{ pushMessage(USER_CMD, new UserCmd(userCmd)); }
		void pushMessage(const ShootCmd &shootCmd) const					{ pushMessage(SHOOT_CMD, new ShootCmd(shootCmd)); }
		
		void pushMessage(const UpdatePlayerObj &updatePlayerObj) const		{ pushMessage(UPDATE_PLAYER_OBJ,  new UpdatePlayerObj(updatePlayerObj)); }
		void pushMessage(const AddPlayerObj &addPlayerObj) const			{ pushMessage(ADD_PLAYER_OBJ,  new AddPlayerObj(addPlayerObj)); }
		void pushMessage(const AddObstacle &addObstacle) const				{ pushMessage(ADD_OBSTACLE,  new AddObstacle(addObstacle)); }
		void pushMessage(const AddProjectile &addProjectile) const			{ pushMessage(ADD_PROJECTILE, new AddProjectile(addProjectile)); }
		void pushMessage(const UpdateProjectile &updateProjectile) const	{ pushMessage(UPDATE_PROJECTILE, new UpdateProjectile(updateProjectile)); }
		void pushMessage(const RemoveProjectile &removeProjectile) const	{ pushMessage(REMOVE_PROJECTILE, new RemoveProjectile(removeProjectile)); }
		void pushMessage(const Kill &kill) const							{ pushMessage(KILL, new Kill(kill)); }
		
		void pushMessage(const InitClient &initClient) const				{ pushMessage(INIT_CLIENT, new InitClient(initClient)); }
		void pushMessage(const WelcomeClient &welcomeClient) const			{ pushMessage(WELCOME_CLIENT, new WelcomeClient(welcomeClient)); }



		// --------------------------------- recieving messages ------------------------------

		//int getNMessages() const; // returns number of recieve messages on the queue
		bool hasMessageOnQueue() const; // returns true if at least 1 message is on recieve queue
		int popMessage(); // returns the type member of the popped message, (will destroy the last message)	
		int getPoppedType() const; // returns the type member of the popped message		
		
		
		UserCmd* getPoppedUserCmd()	const						{ return reinterpret_cast<UserCmd*>(getPoppedData()); }
		ShootCmd* getPoppedShootCmd()	const					{ return reinterpret_cast<ShootCmd*>(getPoppedData()); }
		UpdatePlayerObj* getPoppedUpdatePlayerObj() const		{ return reinterpret_cast<UpdatePlayerObj*>(getPoppedData()); }
		AddPlayerObj* getPoppedAddPlayerObj() const				{ return reinterpret_cast<AddPlayerObj*>(getPoppedData()); }
		AddObstacle* getPoppedAddObstacle() const				{ return reinterpret_cast<AddObstacle*>(getPoppedData()); }
		AddProjectile* getPoppedAddProjectile() const			{ return reinterpret_cast<AddProjectile*>(getPoppedData()); }
		UpdateProjectile* getPoppedUpdateProjectile() const		{ return reinterpret_cast<UpdateProjectile*>(getPoppedData()); }
		RemoveProjectile* getPoppedRemoveProjectile() const		{ return reinterpret_cast<RemoveProjectile*>(getPoppedData()); }
		Kill* getPoppedKill() const								{ return reinterpret_cast<Kill*>(getPoppedData()); }
		InitClient* getPoppedInitClient() const					{ return reinterpret_cast<InitClient*>(getPoppedData()); }
		WelcomeClient* getPoppedWelcomeClient() const			{ return reinterpret_cast<WelcomeClient*>(getPoppedData()); }

	};



};

#endif
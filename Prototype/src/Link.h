#ifndef __Link_h__
#define __Link_h__

#include "messages.h"
#include "Rectangle.h"
#include "Color.h"

namespace Prototype
{
	enum MessageType
	{
		// server
		UPDATE_PLAYER_OBJ = 0,
		ADD_PLAYER_OBJ,		
		ADD_OBSTACLE,
		WELCOME_CLIENT,
		ADD_PLAYER,
		ADD_PROJECTILE,
		UPDATE_PROJECTILE,
		REMOVE_PROJECTILE,
		KILL,
		START_GAME,

		// client
		INIT_CLIENT,
		USER_CMD,
		SHOOT_CMD,
	};

	// --------------------------------------------------------------------------------
	// --------------------------------- server messages ------------------------------
	// --------------------------------------------------------------------------------

	struct UpdatePlayerObj
	{
		static const size_t messageType = UPDATE_PLAYER_OBJ;

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
		static const size_t messageType = ADD_PLAYER_OBJ;

		Color color;
		Pos pos;
		size_t playerId;
	
		AddPlayerObj(size_t playerId, const Color &color, const Pos &pos)
			: playerId(playerId), color(color), pos(pos) {}
	};

	struct AddObstacle
	{
		static const size_t messageType = ADD_OBSTACLE;

		size_t obstacleId;
		Rectangle obstacleArea;
		AddObstacle(size_t obstacleId, const Rectangle &obstacleArea)
			: obstacleId(obstacleId), obstacleArea(obstacleArea)
		{}
	};

	struct WelcomeClient
	{
		static const size_t messageType = WELCOME_CLIENT;

		size_t playerId;

		WelcomeClient(size_t playerId) 
			: playerId(playerId) {}
	};

	struct AddPlayer
	{
		static const size_t messageType = ADD_PLAYER;

		size_t playerId;
		Color color;
		Pos startPos;
		
		AddPlayer(size_t playerId, Color color, Pos startPos) 
			: playerId(playerId), color(color), startPos(startPos) {}
	};

	struct AddProjectile
	{
		static const size_t messageType = ADD_PROJECTILE;

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
		static const size_t messageType = UPDATE_PROJECTILE;

		size_t projectileId;
		Pos pos;

		UpdateProjectile(size_t projectileId, Pos pos)
			: projectileId(projectileId), pos(pos)
		{}
	};

	struct RemoveProjectile
	{
		static const size_t messageType = REMOVE_PROJECTILE;

		size_t projectileId;
		Pos hitPosition;

		RemoveProjectile(size_t projectileId, Pos hitPosition)
			: projectileId(projectileId), hitPosition(hitPosition)
		{}
	};

	struct Kill
	{
		static const size_t messageType = KILL;

		size_t killerId;
		size_t killedId;
		Pos respawnPos;

		Kill(size_t killerId, size_t killedId, Pos respawnPos)
			: killerId(killerId), killedId(killedId), respawnPos(respawnPos)
		{}
	};
	
	struct StartGame
	{
	};

	// --------------------------------------------------------------------------------
	// --------------------------------- client messages ------------------------------
	// --------------------------------------------------------------------------------

	struct InitClient
	{
		static const size_t messageType = INIT_CLIENT;

		Color color;
		
		InitClient(Color color) : color(color) {}
	};

	struct UserCmd
	{
		static const size_t messageType = USER_CMD;

		bool cmdLeft;
		bool cmdRight;
		bool cmdUp;
		bool cmdDown;
		//bool cmdShoot;
		float viewangle;
	};

	struct ShootCmd
	{
		static const size_t messageType = SHOOT_CMD;

		size_t playerId;
		int weapon;
		
		ShootCmd(size_t playerId, int weapon)
			: playerId(playerId), weapon(weapon)
		{}
	};



	/*
	static const size_t N_MESSAGE_TYPES = 13;
	static const size_t sizeOfMessageTypes[N_MESSAGE_TYPES] = { 
		// server
		sizeof(UpdatePlayerObj),
		sizeof(AddPlayerObj),		
		sizeof(AddObstacle),
		sizeof(WelcomeClient),
		sizeof(AddPlayer),
		sizeof(AddProjectile),
		sizeof(UpdateProjectile),
		sizeof(RemoveProjectile),
		sizeof(Kill),
		sizeof(StartGame),
		
		// client
		sizeof(InitClient),
		sizeof(UserCmd),
		sizeof(ShootCmd)
	};
	*/

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
		void pushMessage(int type, int size, void *data) const;

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

		template <class Cmd>
		void pushMessage(const Cmd &cmd) const
		{
			pushMessage(Cmd::messageType, sizeof(Cmd), new Cmd(cmd));
		}
		
		/*
		void pushMessage(const UserCmd &userCmd) const						{ pushMessage(USER_CMD, new UserCmd(userCmd)); }
		void pushMessage(const ShootCmd &shootCmd) const					{ pushMessage(SHOOT_CMD, new ShootCmd(shootCmd)); }
		
		void pushMessage(const UpdatePlayerObj &updatePlayerObj) const		{ pushMessage(UPDATE_PLAYER_OBJ,  new UpdatePlayerObj(updatePlayerObj)); }
		void pushMessage(const AddPlayerObj &addPlayerObj) const			{ pushMessage(ADD_PLAYER_OBJ,  new AddPlayerObj(addPlayerObj)); }
		void pushMessage(const AddObstacle &addObstacle) const				{ pushMessage(ADD_OBSTACLE,  new AddObstacle(addObstacle)); }
		void pushMessage(const AddProjectile &addProjectile) const			{ pushMessage(ADD_PROJECTILE, new AddProjectile(addProjectile)); }
		void pushMessage(const UpdateProjectile &updateProjectile) const	{ pushMessage(UPDATE_PROJECTILE, new UpdateProjectile(updateProjectile)); }
		void pushMessage(const RemoveProjectile &removeProjectile) const	{ pushMessage(REMOVE_PROJECTILE, new RemoveProjectile(removeProjectile)); }
		void pushMessage(const Kill &kill) const							{ pushMessage(KILL, new Kill(kill)); }
		void pushMessage(MessageType messageType) const						{ pushMessage(messageType, 0); }
		
		void pushMessage(const InitClient &initClient) const				{ pushMessage(INIT_CLIENT, new InitClient(initClient)); }
		void pushMessage(const WelcomeClient &welcomeClient) const			{ pushMessage(WELCOME_CLIENT, new WelcomeClient(welcomeClient)); }
		*/


		// --------------------------------- recieving messages ------------------------------

		//int getNMessages() const; // returns number of recieve messages on the queue
		bool hasMessageOnQueue() const; // returns true if at least 1 message is on recieve queue
		int popMessage(); // returns the type member of the popped message, (will destroy the last message)	
		int getPoppedType() const; // returns the type member of the popped message		
		
		template <class Cmd>
		Cmd* getPoppedData() const
		{
			return reinterpret_cast<Cmd*>(getPoppedData());
		}
		
		UserCmd* getPoppedUserCmd()	const						{ return getPoppedData<UserCmd>(); }
		ShootCmd* getPoppedShootCmd()	const					{ return getPoppedData<ShootCmd>(); }
		UpdatePlayerObj* getPoppedUpdatePlayerObj() const		{ return getPoppedData<UpdatePlayerObj>(); }
		AddPlayerObj* getPoppedAddPlayerObj() const				{ return getPoppedData<AddPlayerObj>(); }
		AddObstacle* getPoppedAddObstacle() const				{ return getPoppedData<AddObstacle>(); }
		AddProjectile* getPoppedAddProjectile() const			{ return getPoppedData<AddProjectile>(); }
		UpdateProjectile* getPoppedUpdateProjectile() const		{ return getPoppedData<UpdateProjectile>(); }
		RemoveProjectile* getPoppedRemoveProjectile() const		{ return getPoppedData<RemoveProjectile>(); }
		Kill* getPoppedKill() const								{ return getPoppedData<Kill>(); }
		InitClient* getPoppedInitClient() const					{ return getPoppedData<InitClient>(); }
		WelcomeClient* getPoppedWelcomeClient() const			{ return getPoppedData<WelcomeClient>(); }

		/*
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
		*/
	};
};

#endif

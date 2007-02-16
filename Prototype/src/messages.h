#ifndef __messages_h__
#define __messages_h__

//#include <vector>

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

		//bool cmdLeft;
		//bool cmdRight;
		//bool cmdUp;
		//bool cmdDown;
		//float viewangle;

		int stateCmds; // bitpattern
		float aimangle;

		UserCmd(int stateCmds, float aimangle) : stateCmds(stateCmds), aimangle(aimangle)
		{}

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

};

#endif

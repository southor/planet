#ifndef __messages_h__
#define __messages_h__

//#include <vector>

#include "Rectangle.h"
#include "Color.h"
#include "GameObjId.h"
#include "Angle.h"
#include "StateCmds.h"


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
		SYNC_PONG,

		// client
		INIT_CLIENT,
		SYNC_PING,
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
		PlayerId playerId;
		Pos pos;
		Angle angle;

		UpdatePlayerObj()		{}
		UpdatePlayerObj(PlayerId playerId, const Pos &pos, float angle)
			: playerId(playerId), pos(pos), angle(angle)
		{}
	};
	
	struct AddPlayerObj
	{
		static const size_t messageType = ADD_PLAYER_OBJ;

		Color color;
		Pos pos;
		PlayerId playerId;
	
		AddPlayerObj(PlayerId playerId, const Color &color, const Pos &pos)
			: playerId(playerId), color(color), pos(pos) {}
	};

	struct AddObstacle
	{
		static const size_t messageType = ADD_OBSTACLE;

		GameObjId obstacleId;
		Rectangle obstacleArea;
		AddObstacle(GameObjId obstacleId, const Rectangle &obstacleArea)
			: obstacleId(obstacleId), obstacleArea(obstacleArea)
		{}
	};

	struct WelcomeClient
	{
		static const size_t messageType = WELCOME_CLIENT;

		PlayerId playerId;

		WelcomeClient(PlayerId playerId) 
			: playerId(playerId) {}
	};

	struct AddPlayer
	{
		static const size_t messageType = ADD_PLAYER;

		PlayerId playerId;
		Color color;
		Pos startPos;
		
		AddPlayer(PlayerId playerId, Color color, Pos startPos) 
			: playerId(playerId), color(color), startPos(startPos) {}
	};

	struct AddProjectile
	{
		static const size_t messageType = ADD_PROJECTILE;

		GameObjId projectileId;
		int type;
		Pos pos;
		float angle;
		PlayerId shooterId;

		AddProjectile(GameObjId projectileId, int type, Pos pos, float angle, PlayerId shooterId)
			: projectileId(projectileId), type(type), pos(pos), angle(angle), shooterId(shooterId)
		{}
	};

	struct UpdateProjectile
	{
		static const size_t messageType = UPDATE_PROJECTILE;

		GameObjId projectileId;
		Pos pos;

		UpdateProjectile(GameObjId projectileId, Pos pos)
			: projectileId(projectileId), pos(pos)
		{}
	};

	struct RemoveProjectile
	{
		static const size_t messageType = REMOVE_PROJECTILE;

		GameObjId projectileId;
		Pos hitPosition;

		RemoveProjectile(GameObjId projectileId, Pos hitPosition)
			: projectileId(projectileId), hitPosition(hitPosition)
		{}
	};

	struct Kill
	{
		static const size_t messageType = KILL;

		PlayerId killerId;
		PlayerId killedId;
		Pos respawnPos;

		Kill(PlayerId killerId, PlayerId killedId, Pos respawnPos)
			: killerId(killerId), killedId(killedId), respawnPos(respawnPos)
		{}
	};
	
	struct StartGame
	{
		static const size_t messageType = START_GAME;
	};

	struct SyncPong
	{
		static const size_t messageType = SYNC_PONG;

		int time;
		int pingSendTime;

		SyncPong(int time, int pingSendTime) : time(time), pingSendTime(pingSendTime) {}
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

	struct SyncPing
	{
		static const size_t messageType = SYNC_PING;

		PlayerId playerId;
		int pingSendTime;

		SyncPing(PlayerId playerId, int pingSendTime) : playerId(playerId), pingSendTime(pingSendTime) {}
	};

	struct UserCmd
	{
		static const size_t messageType = USER_CMD;

		//bool cmdLeft;
		//bool cmdRight;
		//bool cmdUp;
		//bool cmdDown;
		//float viewangle;

		//int stateCmds; // bitpattern
		StateCmds stateCmds;
		Angle aimAngle;



		UserCmd()
		{}

		//UserCmd(int stateCmds, Angle aimAngle) : stateCmds(stateCmds), aimAngle(aimAngle)
		UserCmd(StateCmds stateCmds, Angle aimAngle) : stateCmds(stateCmds), aimAngle(aimAngle)
		{}

		
		
		UserCmd operator +(const UserCmd &rh) const
		{			
			
			UserCmd result(stateCmds | rh.stateCmds, aimAngle + rh.aimAngle);
			return result;
		}

		UserCmd operator -(const UserCmd &rh) const
		{
			UserCmd result(stateCmds & rh.stateCmds, aimAngle - rh.aimAngle);
			return result;
		}

		UserCmd operator *(float rh) const
		{			
			return *this;
		}

	};

	struct ShootCmd
	{
		static const size_t messageType = SHOOT_CMD;

		PlayerId playerId;
		int weapon;
		
		ShootCmd(PlayerId playerId, int weapon)
			: playerId(playerId), weapon(weapon)
		{}
	};

};

#endif

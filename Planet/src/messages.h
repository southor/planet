#ifndef __Messages_h__
#define __Messages_h__

#include "Color.h"
#include "basic.h"
#include "Projectile.h"
#include "StateCmds.h"

namespace Planet
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
		PROJECTILE_HIT,
		KILL,
		START_GAME,
		SYNC_PONG,
		SET_TICK_0_TIME,

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
		Vec direction;
		Tickf nextShootTick;
		int ammo[Projectile::N_TYPES];

		UpdatePlayerObj()		{}
		UpdatePlayerObj(PlayerId playerId, const Pos &pos, Vec direction, Tickf nextShootTick, const int ammo[Projectile::N_TYPES])
			: playerId(playerId), pos(pos), direction(direction), nextShootTick(nextShootTick)
		{
			for(int i=0; i<Projectile::N_TYPES; ++i)
			{
				this->ammo[i] = ammo[i];
			}
		}
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

	//struct AddObstacle
	//{
	//	static const size_t messageType = ADD_OBSTACLE;

	//	GameObjId obstacleId;
	//	Rectangle obstacleArea;
	//	AddObstacle(GameObjId obstacleId, const Rectangle &obstacleArea)
	//		: obstacleId(obstacleId), obstacleArea(obstacleArea)
	//	{}
	//};

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
		Vec direction
		GameObjId shooterId;
		Tickf shootTick;
		int objLag;

		AddProjectile(GameObjId projectileId, int type, Pos pos, Vec direction, GameObjId shooterId, Tickf shootTick, int objLag)
			: projectileId(projectileId), type(type), pos(pos), direction(direction), shooterId(shooterId), shootTick(shootTick), objLag(objLag)
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

		RemoveProjectile(GameObjId projectileId)
			: projectileId(projectileId)
		{}
	};

	struct ProjectileHit
	{
		static const size_t messageType = PROJECTILE_HIT;

		GameObjId projectileId;
		Pos hitPosition;

		ProjectileHit(GameObjId projectileId, Pos hitPosition)
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

	struct SetTick0Time
	{
		static const size_t messageType = SET_TICK_0_TIME;

		int tick0Time;

		SetTick0Time(int tick0Time) : tick0Time(tick0Time)
		{}

		inline operator int() const		{ return tick0Time; }
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

		StateCmds stateCmds;
		Pos aimPos;
		
		Projectile::Type weapon;
		int nShots;
		Tickf firstShotTick;
		bool shooting; // keep shooting at the end of this Tick?
		
		int objLag; // The object lag the client uses this tick
		GameObjId firstProjectileId; // the id of the first projectile shot during this tick
		
		UserCmd()
		{}

		UserCmd(StateCmds stateCmds, Pos aimPos, Projectile::Type weapon, int nShots,
				bool shooting, Tickf firstShotTick, GameObjId firstProjectileId, int objLag)
			: stateCmds(stateCmds), aimPos(aimPos), weapon(weapon), nShots(nShots), shooting(shooting),
			  firstShotTick(firstShotTick), firstProjectileId(firstProjectileId), objLag(objLag)
		{}

		
		void clear();

		inline bool isShooting() const					{ return shooting; }
		
		static void interExtraPolate(int tick1, const UserCmd& data1, int tick2, const UserCmd& data2, Tickf resultTick, UserCmd& resultData);

		// Produces the next legal UserCmd from this one.
		void assumeNext(UserCmd &resultData) const;

		static const UserCmd DEFAULT_USER_CMD;

		// debug
		bool isConsistent(int currentTick) const;

	};	

};

#endif

#ifndef __Messages_h__
#define __Messages_h__

#include "Color.h"
#include "basic.h"
#include "Projectile.h"
#include "GameObjId.h"
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

	static const int N_WEAPONS = Projectile::N_TYPES;

	class AmmoSupply
	{
	public:
		short ammo[N_WEAPONS];

		//AmmoSupply operator +(const AmmoSupply &rh) const
		//{
		//	AmmoSupply result;
		//	for(int i=0; i<Projectile::N_TYPES; ++i)
		//	{
		//		result.ammo[i] = this->ammo[i] + rh.ammo[i];
		//	}
		//	
		//	return result;
		//}

		//AmmoSupply operator -(const AmmoSupply &rh) const
		//{
		//	AmmoSupply result;
		//	for(int i=0; i<Projectile::N_TYPES; ++i)
		//	{
		//		result.ammo[i] = this->ammo[i] - rh.ammo[i];
		//	}
		//	
		//	return result;
		//}

		//AmmoSupply operator *(float f) const
		//{
		//	AmmoSupply result;
		//	for(int i=0; i<Projectile::N_TYPES; ++i)
		//	{
		//		result.ammo[i] = static_cast<short>(static_cast<float>(this->ammo[i]) * f);
		//	}
		//	
		//	return result;
		//}

		bool operator !=(const AmmoSupply &rh) const
		{
			for(int i=0; i<Projectile::N_TYPES; ++i)
			{
				if (this->ammo[i] != rh.ammo[i]) return false;
			}
			
			return true;
		}

		const short& operator [](Projectile::Type type) const
		{
			assert((type >= 0) && (type < N_WEAPONS));
			return ammo[type];
		}

		short& operator [](Projectile::Type type)
		{
			assert((type >= 0) && (type < N_WEAPONS));
			return ammo[type];
		}

		bool isConsistent()
		{
			for(int i=0; i<Projectile::N_TYPES; ++i)
			{
				if (ammo[i] < 0) return false;
			}
			return true;
		}
	};

	struct UpdatePlayerObj
	{
		static const size_t messageType = UPDATE_PLAYER_OBJ;

		//size_t playerObjId;
		PlayerId playerId;
		Pos pos;
		//Vec direction;
		Pos aimPos;
		Tickf nextShootTick;
		AmmoSupply ammoSupply;
		//short ammo[Projectile::N_TYPES];

		UpdatePlayerObj()		{}
		UpdatePlayerObj(PlayerId playerId, const Pos &pos,
			//Vec direction,
			Pos aimPos,
			Tickf nextShootTick, const AmmoSupply &ammoSupply)
			: playerId(playerId), pos(pos), aimPos(aimPos), nextShootTick(nextShootTick),
			  ammoSupply(ammoSupply)
		{
			//for(int i=0; i<Projectile::N_TYPES; ++i)
			//{
			//	this->ammo[i] = ammo[i];
			//}
		}

		bool isConsistent()
		{
			if (playerId < 0) return false;
			if (nextShootTick < 0.0) return false;
			return ammoSupply.isConsistent();
		}
	};
	
	struct AddPlayerObj
	{
		static const size_t messageType = ADD_PLAYER_OBJ;

		Color color;
		Pos pos;
		Pos aimPos;
		PlayerId playerId;
	
		AddPlayerObj(PlayerId playerId, const Color &color, const Pos &pos, const Pos &aimPos)
			: playerId(playerId), color(color), pos(pos), aimPos(aimPos)
		{}

		bool isConsistent()
		{
			return playerIdIsConsistent(playerId);
		}
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

		bool isConsistent()
		{
			return playerIdIsConsistent(playerId);
		}
	};

	struct AddPlayer
	{
		static const size_t messageType = ADD_PLAYER;

		PlayerId playerId;
		Color color;
		//Pos startPos;
		
		//AddPlayer(PlayerId playerId, Color color, Pos startPos) 
		//	: playerId(playerId), color(color), startPos(startPos) {}
		AddPlayer(PlayerId playerId, Color color) 
			: playerId(playerId), color(color)
		{}

		bool isConsistent()
		{
			return playerIdIsConsistent(playerId);
		}
	};

	struct AddProjectile
	{
		static const size_t messageType = ADD_PROJECTILE;

		GameObjId projectileId;
		int type;
		Pos pos;
		Vec ext;
		GameObjId shooterId;
		Tickf shootTick;
		int objLag;

		AddProjectile(GameObjId projectileId, int type, Pos pos, Vec ext, GameObjId shooterId, Tickf shootTick, int objLag)
			: projectileId(projectileId), type(type), pos(pos), ext(ext), shooterId(shooterId), shootTick(shootTick), objLag(objLag)
		{}

		bool isConsistent()
		{
			if (!projectileId.isConsistent()) return false;
			if ((type < 0) || (type >= Projectile::N_TYPES)) return false;
			if (!shooterId.isConsistent()) return false;
			if (shootTick < 0.0) return false;
			if (objLag < 0) return false;
			return true;
		}
	};

	struct UpdateProjectile
	{
		static const size_t messageType = UPDATE_PROJECTILE;

		GameObjId projectileId;
		Pos pos;

		UpdateProjectile(GameObjId projectileId, Pos pos)
			: projectileId(projectileId), pos(pos)
		{}

		bool isConsistent()
		{
			return projectileId.isConsistent();
		}
	};

	struct RemoveProjectile
	{
		static const size_t messageType = REMOVE_PROJECTILE;

		GameObjId projectileId;

		RemoveProjectile(GameObjId projectileId)
			: projectileId(projectileId)
		{}

		bool isConsistent()
		{
			return projectileId.isConsistent();
		}
	};

	struct ProjectileHit
	{
		static const size_t messageType = PROJECTILE_HIT;

		GameObjId projectileId;
		Pos hitPosition;

		ProjectileHit(GameObjId projectileId, Pos hitPosition)
			: projectileId(projectileId), hitPosition(hitPosition)
		{}

		bool isConsistent()
		{
			return projectileId.isConsistent();
		}
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

		bool isConsistent()
		{
			if (!playerIdIsConsistent(killerId)) return false;
			if (!playerIdIsConsistent(killedId)) return false;
			return true;
		}
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

		SyncPong(int time, int pingSendTime)
			: time(time), pingSendTime(pingSendTime)
		{}
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

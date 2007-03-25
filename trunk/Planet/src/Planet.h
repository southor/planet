#ifndef __Planet_h__
#define __Planet_h__

//#include "Obstacle.h"
#include "PlayerObj.h"
#include "Projectile.h"
#include "PlanetBody.h"

#include "IdMap.h"
#include "GameObjId.h"

#include <vector>
#include <list>

namespace Planet
{
	class Planet
	{
	public:
		

		static const Vec WORLD_SIZE;		

		//typedef IdMap<GameObjId, Obstacle*> Obstacles;
		typedef IdMap<GameObjId, PlayerObj*> PlayerObjs;
		typedef IdMap<GameObjId, Projectile*> Projectiles;


		Planet();

		virtual ~Planet()		{}

		void deleteAllObjs();
		
		//virtual Obstacles& getObstacles() = 0;
		inline PlayerObjs& getPlayerObjs()						{ return playerObjs; }
		inline Projectiles& getProjectiles()					{ return projectiles; }

		//virtual const Obstacles& getObstacles() const = 0;
		inline const PlayerObjs& getPlayerObjs() const			{ return playerObjs; }
		inline const Projectiles& getProjectiles() const		{ return projectiles; }

		// history functions
		void updatePlayerObjsToTickData(Tickf tick);

		/**
		 * @param projectileId The id that the projectile should get		 
		 */
		void playerShoot(PlayerId playerId, Projectile::Type weapon, Tickf shootTick, int objLag, GameObjId projectileId);

		/**
		 * @param projectileId The id that the projectile should get
		 * @param shotTick The tick that the shot will be shot
		 * @return true if projectile was created		 
		 */
		bool playerTryShoot(PlayerId playerId, int currentTick, int shotN, GameObjId projectileId, Tickf &shotTick);

		inline bool playerTryShoot(PlayerId playerId, int currentTick, int shotN, GameObjId projectileId)
		{
			Tickf dummyTick;
			return playerTryShoot(playerId, currentTick, shotN, projectileId, dummyTick);
		}

		// TODO
		void updatePlayerObjMovements();
		void updatePlayerObjMovement(PlayerId playerId);
		void updateProjectileMovements()						{ assert(true) ;}
		
		void init(const std::string &map)						{ planetBody.init(map); }

		inline PlanetBody* getPlanetBody()						{ return &planetBody; }
		
		

		bool isConsistent();

	protected:

		PlanetBody planetBody;

		PlayerObjs playerObjs;
		Projectiles projectiles;


		//class Move
		//{
		//protected:
		//	Obstacles *obstacles;
		//	
		//	Obstacle* findAnyOverlap(const Rectangle &rectangle);
		//public:
		//	// @param deltaTime Time in milliseconds since last move.
		//	Move(Obstacles *obstacles)
		//		: obstacles(obstacles)
		//	{}

		//	//// @param deltaTime Time in milliseconds since last move.
		//	//Move(Obstacles *obstacles, PlayerObjs *playerObjs, float deltaTime)
		//	//	: obstacles(obstacles), playerObjs(playerObjs), deltaTime(deltaTime), moveAlignedToAngle(false)
		//	//{}
		//};

		//class MovePlayerObj : public Move
		//{
		//protected:
		//	bool moveAlignedToAngle;
		//public:
		//	// @param deltaTime Time in milliseconds since last move.
		//	MovePlayerObj(Obstacles *obstacles, bool moveAlignedToAngle)
		//		: Move(obstacles), moveAlignedToAngle(moveAlignedToAngle)
		//	{}

		//	void operator ()(const PlayerObjs::Pair &playerObjPair);
		//};
	};
};

#endif

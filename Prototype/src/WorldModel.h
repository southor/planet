#ifndef __worldmodel_h__
#define __worldmodel_h__

#include "Obstacle.h"
#include "PlayerObj.h"
#include "Projectile.h"
#include "ServerPlayers.h"

#include "IdMap.h"
#include "GameObjId.h"

#include <vector>
#include <list>

namespace Prototype
{

	//class ServerPlayers;

	class WorldModel
	{
	public:

		static const bool MOVE_ALIGNED_TO_ANGLE_DEFAULT = false;
		bool moveAlignedToAngle;
		

		static const Vec WORLD_SIZE;		

		typedef IdMap<GameObjId, Obstacle*> Obstacles;
		typedef IdMap<GameObjId, PlayerObj*> PlayerObjs;
		typedef IdMap<GameObjId, Projectile*> Projectiles;


		WorldModel() : moveAlignedToAngle(MOVE_ALIGNED_TO_ANGLE_DEFAULT)
		{}

		virtual ~WorldModel()		{}

		void deleteAllObjs();
		
		virtual Obstacles& getObstacles() = 0;
		virtual PlayerObjs& getPlayerObjs() = 0;
		virtual Projectiles& getProjectiles() = 0;

		virtual const Obstacles& getObstacles() const = 0;
		virtual const PlayerObjs& getPlayerObjs() const = 0;
		virtual const Projectiles& getProjectiles() const = 0;

		// history functions
		//void storeToTickData(int tick);
		//void updatePlayerObjsToTickData(int tick);
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
		

		bool performProjectileHit(GameObjId projectileId, ProjectileHit &projectileHit);



		bool isConsistent();

	protected:

		//class Delete
		//{
		//public:
		//	void operator ()(const Obstacles::Pair &obstaclePair)		{ delete obstaclePair.second; }
		//	void operator ()(const PlayerObjs::Pair &playerObjPair)		{ delete playerObjPair.second; }
		//	void operator ()(const Projectiles::Pair &projectilePair)	{ delete projectilePair.second; }
		//};

		class DeleteObstacle
		{
		public:
			void operator ()(const Obstacles::Pair &obstaclePair)		{ delete obstaclePair.second; }
		};

		class DeletePlayerObj
		{
		public:
			void operator ()(const PlayerObjs::Pair &playerObjPair)		{ delete playerObjPair.second; }
		};

		class DeleteProjectile
		{
		public:
			void operator ()(const Projectiles::Pair &projectilePair)	{ delete projectilePair.second; }
		};


		class Move
		{
		protected:
			Obstacles *obstacles;
			
			Obstacle* findAnyOverlap(const Rectangle &rectangle);
		public:
			// @param deltaTime Time in milliseconds since last move.
			Move(Obstacles *obstacles)
				: obstacles(obstacles)
			{}

			//// @param deltaTime Time in milliseconds since last move.
			//Move(Obstacles *obstacles, PlayerObjs *playerObjs, float deltaTime)
			//	: obstacles(obstacles), playerObjs(playerObjs), deltaTime(deltaTime), moveAlignedToAngle(false)
			//{}
		};

		class MovePlayerObj : public Move
		{
		protected:
			bool moveAlignedToAngle;
		public:
			// @param deltaTime Time in milliseconds since last move.
			MovePlayerObj(Obstacles *obstacles, bool moveAlignedToAngle)
				: Move(obstacles), moveAlignedToAngle(moveAlignedToAngle)
			{}

			void operator ()(const PlayerObjs::Pair &playerObjPair);
		};
	};
};

#endif

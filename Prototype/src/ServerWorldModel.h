#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"
#include "ServerPlayers.h"

#include <vector>

namespace Prototype
{
	class ServerWorldModel : public WorldModel
	{
	private:

		//typedef std::vector<Obstacle*> ServerObstacleContainer;
		//typedef std::list<PlayerObj*> ServerPlayerObjContainer;
		//typedef std::list<Projectile*> ServerProjectileContainer;
		typedef IdMap<size_t, Obstacle*> ServerObstacleContainer;
		typedef IdMap<size_t, PlayerObj*> ServerPlayerObjContainer;
		typedef IdMap<size_t, Projectile*> ServerProjectileContainer;		

		ServerObstacleContainer obstacles;
		ServerPlayerObjContainer playerObjs;
		ServerProjectileContainer projectiles;

		typedef std::vector<Pos> RespawnPoss;
		RespawnPoss respawnPoss;

		class Move
		{
		private:
			ServerObstacleContainer *obstacles;
			float deltaTime; // Time in milliseconds since last move.
			ServerPlayers *players;
			ServerPlayerObjContainer *playerObjs;
			std::vector<RemoveProjectile> projectilesHit;
			RespawnPoss *respawnPoss;
			bool moveAlignedToAngle;

			Obstacle* findAnyOverlap(const Rectangle &rectangle);
		public:
			// @param deltaTime Time in milliseconds since last move.
			Move(ServerObstacleContainer *obstacles, ServerPlayers *players, ServerPlayerObjContainer *playerObjs, RespawnPoss *respawnPoss, float deltaTime)
				: obstacles(obstacles), players(players), playerObjs(playerObjs), respawnPoss(respawnPoss), deltaTime(deltaTime), moveAlignedToAngle(false)
			{}

			// @param deltaTime Time in milliseconds since last move.
			Move(ServerObstacleContainer *obstacles, float deltaTime, bool moveAlignedToAngle)
				: obstacles(obstacles), players(0), playerObjs(0), respawnPoss(0), deltaTime(deltaTime), moveAlignedToAngle(moveAlignedToAngle)
			{}

			inline std::vector<RemoveProjectile>& getProjectilesHit()	
			{
				return projectilesHit;
			}

			void operator ()(const PlayerObjContainer::Pair &playerObjPair);
			void operator ()(const ProjectileContainer::Pair &projectilePair);
		};

	public:

		bool moveAlignedToAngle;

		ServerWorldModel() : moveAlignedToAngle(false)
		{}

		~ServerWorldModel();
		
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }


		void addPlayerObj(size_t playerId, const Pos &playerPos);
		size_t addObstacle(const Rectangle &obstacleArea);

		void addRespawnPos(Pos pos)						{ respawnPoss.push_back(pos); }
		
		void updatePlayerObjMovements(float deltaTime);
		void updateProjectileMovements(float deltaTime, ServerPlayers &players);

		// @retunr projectileId
		size_t playerShoot(size_t playerId, Projectile::Type weapon);
	};
};

#endif
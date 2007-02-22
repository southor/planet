#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"
#include "ServerPlayers.h"
#include "ServerGlobalAccess.h"

#include <vector>

namespace Prototype
{
	class ServerWorldModel : public WorldModel, public ServerGlobalAccess
	{
	private:

		//typedef std::vector<Obstacle*> ServerObstacleContainer;
		//typedef std::list<PlayerObj*> ServerPlayerObjContainer;
		//typedef std::list<Projectile*> ServerProjectileContainer;
		typedef IdMap<GameObjId, Obstacle*> ServerObstacleContainer;
		typedef IdMap<GameObjId, PlayerObj*> ServerPlayerObjContainer;
		typedef IdMap<GameObjId, Projectile*> ServerProjectileContainer;		

		ServerObstacleContainer obstacles;
		ServerPlayerObjContainer playerObjs;
		ServerProjectileContainer projectiles;

		typedef std::vector<Pos> RespawnPoss;
		RespawnPoss respawnPoss;

		class Move : public ServerGlobalAccess
		{
		protected:
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
			Move(ServerObstacleContainer *obstacles, const ServerGlobalAccess &serverGlobalAccess, float deltaTime, bool moveAlignedToAngle)
				: obstacles(obstacles), players(0), ServerGlobalAccess(serverGlobalAccess), playerObjs(0), respawnPoss(0), deltaTime(deltaTime), moveAlignedToAngle(moveAlignedToAngle)
			{}

			// @param deltaTime Time in milliseconds since last move.
			Move(ServerObstacleContainer *obstacles, ServerPlayers *players, const ServerGlobalAccess &serverGlobalAccess, ServerPlayerObjContainer *playerObjs, RespawnPoss *respawnPoss, float deltaTime)
				: obstacles(obstacles), players(players), ServerGlobalAccess(serverGlobalAccess), playerObjs(playerObjs), respawnPoss(respawnPoss), deltaTime(deltaTime), moveAlignedToAngle(false)
			{}

			inline std::vector<RemoveProjectile>& getProjectilesHit()	
			{
				return projectilesHit;
			}
		};

		class MovePlayerObj : public Move
		{
		public:
			// @param deltaTime Time in milliseconds since last move.
			MovePlayerObj(ServerObstacleContainer *obstacles, const ServerGlobalAccess &serverGlobalAccess, float deltaTime, bool moveAlignedToAngle)
				: Move(obstacles, serverGlobalAccess, deltaTime, moveAlignedToAngle)
			{}

			void operator ()(const PlayerObjContainer::Pair &playerObjPair);
		};

		class MoveProjectile : public Move
		{
		public:
			// @param deltaTime Time in milliseconds since last move.
			MoveProjectile(ServerObstacleContainer *obstacles, ServerPlayers *players, const ServerGlobalAccess &serverGlobalAccess, ServerPlayerObjContainer *playerObjs, RespawnPoss *respawnPoss, float deltaTime)
				: Move(obstacles, players, serverGlobalAccess, playerObjs, respawnPoss, deltaTime)
			{}

			void operator ()(const ProjectileContainer::Pair &projectilePair);
		};

	public:

		bool moveAlignedToAngle;

		ServerWorldModel(ServerGlobalObj *serverGlobalObj)
			: ServerGlobalAccess(serverGlobalObj), moveAlignedToAngle(false)
		{}

		~ServerWorldModel();
		
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }


		void addPlayerObj(PlayerId playerId, const Pos &playerPos);
		GameObjId addObstacle(const Rectangle &obstacleArea);

		void addRespawnPos(Pos pos)						{ respawnPoss.push_back(pos); }
		
		void updatePlayerObjMovements(float deltaTime);
		void updateProjectileMovements(float deltaTime, ServerPlayers &players);

		// @retunr projectileId
		GameObjId playerShoot(PlayerId playerId, Projectile::Type weapon);
	};
};

#endif

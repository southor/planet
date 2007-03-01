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


/*		typedef IdMap<GameObjId, Obstacle*> ServerObstacleContainer;
		typedef IdMap<GameObjId, PlayerObj*> ServerPlayerObjContainer;
		typedef IdMap<GameObjId, Projectile*> ServerProjectileContainer;	*/	

		ObstacleContainer obstacles;
		PlayerObjContainer playerObjs;
		ProjectileContainer projectiles;

		typedef std::vector<Pos> RespawnPoss;
		RespawnPoss respawnPoss;



		class MoveProjectile : public Move, public ServerGlobalAccess
		{
		private:

			RespawnPoss *respawnPoss;
			ServerPlayers *players;
			PlayerObjContainer *playerObjs;

			std::vector<RemoveProjectile> projectilesHit;

		public:
			// @param deltaTime Time in milliseconds since last move.
			MoveProjectile(ObstacleContainer *obstacles, ServerPlayers *players, const ServerGlobalAccess &serverGlobalAccess, PlayerObjContainer *playerObjs, RespawnPoss *respawnPoss, float deltaTime)
				: Move(obstacles, deltaTime), ServerGlobalAccess(serverGlobalAccess), respawnPoss(respawnPoss), players(players), playerObjs(playerObjs)
			{}

			void operator ()(const ProjectileContainer::Pair &projectilePair);

			inline std::vector<RemoveProjectile>& getProjectilesHit()	
			{
				return projectilesHit;
			}
		};

	public:

		

		ServerWorldModel(ServerGlobalObj *serverGlobalObj)
			: ServerGlobalAccess(serverGlobalObj)
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

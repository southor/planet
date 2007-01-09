#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"
#include "ServerIdMap.h"

namespace Prototype
{
	class ServerWorldModel : public WorldModel
	{
	private:

		//typedef std::vector<Obstacle*> ServerObstacleContainer;
		//typedef std::list<PlayerObj*> ServerPlayerObjContainer;
		//typedef std::list<Projectile*> ServerProjectileContainer;
		typedef ServerIdMap<size_t, Obstacle*> ServerObstacleContainer;
		typedef ServerIdMap<size_t, PlayerObj*> ServerPlayerObjContainer;
		typedef ServerIdMap<size_t, Projectile*> ServerProjectileContainer;

		ServerObstacleContainer obstacles;
		ServerPlayerObjContainer playerObjs;
		ServerProjectileContainer projectiles;

		class Move
		{
		private:
			ServerObstacleContainer *obstacles;

			Obstacle* findAnyOverlap(const Rectangle &rectangle);
		public:
			Move(ServerObstacleContainer *obstacles) : obstacles(obstacles)	{}
			void operator ()(const PlayerObjContainer::Pair &playerObjPair);
		};

	public:

		~ServerWorldModel();
		
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }


		size_t addPlayerObj(size_t playerId, const Pos &playerPos);

		void updatePlayerObjMovements();
	};
};

#endif
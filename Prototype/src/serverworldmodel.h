#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"

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

		class Move
		{
		private:
			ServerObstacleContainer *obstacles;
			float deltaTime; // Time in seconds since last move.

			Obstacle* findAnyOverlap(const Rectangle &rectangle);
		public:
			// @param deltaTime Time in seconds since last move.
			Move(ServerObstacleContainer *obstacles, float deltaTime)
				: obstacles(obstacles), deltaTime(deltaTime)
			{}

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


		void addPlayerObj(size_t playerId, const Pos &playerPos);
		size_t addObstacle(const Rectangle &obstacleArea);

		// @param deltaTime Time in seconds since last move.
		void updatePlayerObjMovements(float deltaTime);
	};
};

#endif
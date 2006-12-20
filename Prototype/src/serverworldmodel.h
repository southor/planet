#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"

namespace Prototype
{
	class ServerWorldModel : public WorldModel
	{
	private:

		typedef std::vector<Obstacle*> ServerObstacleContainer;
		typedef std::list<PlayerObj*> ServerPlayerObjContainer;
		typedef std::list<Projectile*> ServerProjectileContainer;

		ServerObstacleContainer obstacles;
		ServerPlayerObjContainer playerObjs;
		ServerProjectileContainer projectiles;

	public:

		~ServerWorldModel();
		
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }

	};
};

#endif
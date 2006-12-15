#ifndef __worldmodel_h__
#define __worldmodel_h__

#include "obstacle.h"
#include "playerobj.h"
#include "projectile.h"

#include <vector>
#include <list>

namespace Prototype
{
	class WorldModel
	{
	public:
		typedef std::vector<Obstacle*> ObstacleContainer;
		typedef std::list<PlayerObj*> PlayerObjContainer;
		typedef std::list<Projectile*> ProjectileContainer;

	private:
		ObstacleContainer obstacles;
		PlayerObjContainer playerObjs;
		ProjectileContainer projectiles;

	public:
		
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }

	};
};

#endif
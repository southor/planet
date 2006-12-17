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

		static const Vec WORLD_SIZE;		

		typedef std::vector<Obstacle*> ObstacleContainer;
		typedef std::list<PlayerObj*> PlayerObjContainer;
		typedef std::list<Projectile*> ProjectileContainer;

		virtual ~WorldModel();

	private:
		ObstacleContainer obstacles;
		PlayerObjContainer playerObjs;
		ProjectileContainer projectiles;

		class Delete
		{
		public:
			void operator ()(const Obstacle* obstacle)		{ delete obstacle; }
			void operator ()(const PlayerObj* playerObj)	{ delete playerObj; }
			void operator ()(const Projectile* projectile)	{ delete projectile; }
		};

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
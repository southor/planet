#ifndef __clientworldmodel_h__
#define __clientworldmodel_h__

#include "WorldModel.h"


namespace Prototype
{
	class ClientWorldModel : public WorldModel
	{
	private:
		
		typedef std::vector<Obstacle*> ClientObstacleContainer;
		typedef std::list<PlayerObj*> ClientPlayerObjContainer;
		typedef std::list<Projectile*> ClientProjectileContainer;

		ClientObstacleContainer obstacles;
		ClientPlayerObjContainer playerObjs;
		ClientProjectileContainer projectiles;

	public:

		~ClientWorldModel();
				
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }

	};
};

#endif
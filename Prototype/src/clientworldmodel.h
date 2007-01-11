#ifndef __clientworldmodel_h__
#define __clientworldmodel_h__

#include "WorldModel.h"
#include "ClientIdMap.h"

namespace Prototype
{
	class ClientWorldModel : public WorldModel
	{
	private:
		
		//typedef std::vector<Obstacle*> ClientObstacleContainer;
		//typedef std::list<PlayerObj*> ClientPlayerObjContainer;
		//typedef std::list<Projectile*> ClientProjectileContainer;
		typedef ClientIdMap<size_t, Obstacle*> ClientObstacleContainer;
		typedef ClientIdMap<size_t, PlayerObj*> ClientPlayerObjContainer;
		typedef ClientIdMap<size_t, Projectile*> ClientProjectileContainer;

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


		void addPlayerObj(size_t playerId, size_t playerObjId, const Pos &playerPos);
		void addObstacle(size_t obstacleId, const Rectangle &obstacleArea);

	};
};

#endif
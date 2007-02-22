#ifndef __clientworldmodel_h__
#define __clientworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"

namespace Prototype
{
	class ClientWorldModel : public WorldModel
	{
	private:
		
		//typedef std::vector<Obstacle*> ClientObstacleContainer;
		//typedef std::list<PlayerObj*> ClientPlayerObjContainer;
		//typedef std::list<Projectile*> ClientProjectileContainer;
		typedef IdMap<GameObjId, Obstacle*> ClientObstacleContainer;
		typedef IdMap<GameObjId, PlayerObj*> ClientPlayerObjContainer;
		typedef IdMap<GameObjId, Projectile*> ClientProjectileContainer;

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


		//void addPlayerObj(size_t playerId, size_t playerObjId, const Pos &playerPos);
		void addPlayerObj(PlayerId playerId, const Pos &playerPos);
		void addObstacle(GameObjId obstacleId, const Rectangle &obstacleArea);
		void addProjectile(GameObjId projectileId, Projectile::Type type, const Pos &pos, float angle, PlayerId shooterId);

	};
};

#endif

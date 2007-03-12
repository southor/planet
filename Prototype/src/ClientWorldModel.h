#ifndef __clientworldmodel_h__
#define __clientworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"

namespace Prototype
{
	class ClientWorldModel : public WorldModel
	{
	private:
		

		//typedef IdMap<GameObjId, Obstacle*> ClientObstacleContainer;
		//typedef IdMap<GameObjId, PlayerObj*> ClientPlayerObjContainer;
		//typedef IdMap<GameObjId, Projectile*> ClientProjectileContainer;

		ObstacleContainer obstacles;
		PlayerObjContainer playerObjs;
		ProjectileContainer projectiles;

	public:

		~ClientWorldModel();
				
		ObstacleContainer& getObstacles()						{ return obstacles; }
		PlayerObjContainer& getPlayerObjs()						{ return playerObjs; }
		ProjectileContainer& getProjectiles()					{ return projectiles; }

		const ObstacleContainer& getObstacles() const			{ return obstacles; }
		const PlayerObjContainer& getPlayerObjs() const			{ return playerObjs; }
		const ProjectileContainer& getProjectiles() const		{ return projectiles; }


		//void addPlayerObj(size_t playerId, size_t playerObjId, const Pos &playerPos);
		void addPlayerObj(PlayerId playerId, const Pos &playerPos, bool isMe, int tick);
		void addObstacle(GameObjId obstacleId, const Rectangle &obstacleArea);
		void addProjectile(GameObjId projectileId, Projectile::Type type, const Pos &pos, float angle, PlayerId shooterId, Tickf shootTick);

		void updatePlayerObjMovement(PlayerId playerId, float deltaTime);

	};
};

#endif

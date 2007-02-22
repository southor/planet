#ifndef __worldmodel_h__
#define __worldmodel_h__

#include "Obstacle.h"
#include "PlayerObj.h"
#include "Projectile.h"

#include "IdMap.h"
#include "GameObjId.h"

#include <vector>
#include <list>

namespace Prototype
{
	class WorldModel
	{
	public:

		static const Vec WORLD_SIZE;		

		//typedef std::vector<Obstacle*> ObstacleContainer;
		//typedef std::list<PlayerObj*> PlayerObjContainer;
		//typedef std::list<Projectile*> ProjectileContainer;
		typedef IdMap<GameObjId, Obstacle*> ObstacleContainer;
		typedef IdMap<GameObjId, PlayerObj*> PlayerObjContainer;
		typedef IdMap<GameObjId, Projectile*> ProjectileContainer;



		virtual ~WorldModel()		{}

		void deleteAllObjs();
		
		virtual ObstacleContainer& getObstacles() = 0;
		virtual PlayerObjContainer& getPlayerObjs() = 0;
		virtual ProjectileContainer& getProjectiles() = 0;

		virtual const ObstacleContainer& getObstacles() const = 0;
		virtual const PlayerObjContainer& getPlayerObjs() const = 0;
		virtual const ProjectileContainer& getProjectiles() const = 0;

		bool isConsistent();

	protected:

		//class Delete
		//{
		//public:
		//	void operator ()(const ObstacleContainer::Pair &obstaclePair)		{ delete obstaclePair.second; }
		//	void operator ()(const PlayerObjContainer::Pair &playerObjPair)		{ delete playerObjPair.second; }
		//	void operator ()(const ProjectileContainer::Pair &projectilePair)	{ delete projectilePair.second; }
		//};

		class DeleteObstacle
		{
		public:
			void operator ()(const ObstacleContainer::Pair &obstaclePair)		{ delete obstaclePair.second; }
		};

		class DeletePlayerObj
		{
		public:
			void operator ()(const PlayerObjContainer::Pair &playerObjPair)		{ delete playerObjPair.second; }
		};

		class DeleteProjectile
		{
		public:
			void operator ()(const ProjectileContainer::Pair &projectilePair)	{ delete projectilePair.second; }
		};
	};
};

#endif

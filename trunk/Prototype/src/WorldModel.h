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

		static const bool MOVE_ALIGNED_TO_ANGLE_DEFAULT = false;
		bool moveAlignedToAngle;
		

		static const Vec WORLD_SIZE;		

		//typedef std::vector<Obstacle*> ObstacleContainer;
		//typedef std::list<PlayerObj*> PlayerObjContainer;
		//typedef std::list<Projectile*> ProjectileContainer;
		typedef IdMap<GameObjId, Obstacle*> ObstacleContainer;
		typedef IdMap<GameObjId, PlayerObj*> PlayerObjContainer;
		typedef IdMap<GameObjId, Projectile*> ProjectileContainer;


		WorldModel() : moveAlignedToAngle(MOVE_ALIGNED_TO_ANGLE_DEFAULT)
		{}

		virtual ~WorldModel()		{}

		void deleteAllObjs();
		
		virtual ObstacleContainer& getObstacles() = 0;
		virtual PlayerObjContainer& getPlayerObjs() = 0;
		virtual ProjectileContainer& getProjectiles() = 0;

		virtual const ObstacleContainer& getObstacles() const = 0;
		virtual const PlayerObjContainer& getPlayerObjs() const = 0;
		virtual const ProjectileContainer& getProjectiles() const = 0;

		// history functions
		void storeToTickData(int tick);
		void updateToTickData(int tick);
		void updateToTickData(Tickf tick);

		

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


		class Move
		{
		protected:
			ObstacleContainer *obstacles;
			float deltaTime; // Time in milliseconds since last move.
			
			Obstacle* findAnyOverlap(const Rectangle &rectangle);
		public:
			// @param deltaTime Time in milliseconds since last move.
			Move(ObstacleContainer *obstacles, float deltaTime)
				: obstacles(obstacles), deltaTime(deltaTime)
			{}

			//// @param deltaTime Time in milliseconds since last move.
			//Move(ObstacleContainer *obstacles, PlayerObjContainer *playerObjs, float deltaTime)
			//	: obstacles(obstacles), playerObjs(playerObjs), deltaTime(deltaTime), moveAlignedToAngle(false)
			//{}
		};

		class MovePlayerObj : public Move
		{
		protected:
			bool moveAlignedToAngle;
		public:
			// @param deltaTime Time in milliseconds since last move.
			MovePlayerObj(ObstacleContainer *obstacles, float deltaTime, bool moveAlignedToAngle)
				: Move(obstacles, deltaTime), moveAlignedToAngle(moveAlignedToAngle)
			{}

			void operator ()(const PlayerObjContainer::Pair &playerObjPair);
		};
	};
};

#endif

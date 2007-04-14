#ifndef __Planet_h__
#define __Planet_h__

#include "PlayerObj.h"
#include "Projectile.h"
#include "PlanetBody.h"

#include "IdMap.h"
#include "GameObjId.h"

#include <vector>
#include <list>

namespace Planet
{
	class Planet
	{
	public:
		

		static const Vec WORLD_SIZE;		

		typedef IdMap<GameObjId, PlayerObj*> PlayerObjs;
		typedef IdMap<GameObjId, Projectile*> Projectiles;


		Planet();

		virtual ~Planet()		{}

		void deleteAllObjs();
		
		inline PlayerObjs& getPlayerObjs()						{ return playerObjs; }
		inline Projectiles& getProjectiles()					{ return projectiles; }

		inline const PlayerObjs& getPlayerObjs() const			{ return playerObjs; }
		inline const Projectiles& getProjectiles() const		{ return projectiles; }

		// history functions
		void updatePlayerObjsToTickData(Tickf tick);

		/**
		 * @param projectileId The id that the projectile should get		 
		 */
		void playerShoot(PlayerId playerId, Projectile::Type weapon, Tickf shootTick, int objLag, GameObjId projectileId);

		/**
		 * @param projectileId The id that the projectile should get
		 * @param shotTick The tick that the shot will be shot
		 * @return true if projectile was created		 
		 */
		bool playerTryShoot(PlayerId playerId, int currentTick, int shotN, GameObjId projectileId, Tickf &shotTick);

		inline bool playerTryShoot(PlayerId playerId, int currentTick, int shotN, GameObjId projectileId)
		{
			Tickf dummyTick;
			return playerTryShoot(playerId, currentTick, shotN, projectileId, dummyTick);
		}

		void updatePlayerObjMovements();
		void updatePlayerObjMovement(PlayerId playerId);

		// TODO
		void updateProjectileMovements()						{ assert(true) ;}
		
		inline PlanetBody* getPlanetBody()						{ return &planetBody; }

		bool isConsistent();

	protected:

		PlanetBody planetBody;

		PlayerObjs playerObjs;
		Projectiles projectiles;
	};
};

#endif

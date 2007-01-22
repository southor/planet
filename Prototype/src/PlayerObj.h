#ifndef __playerobj_h__
#define __playerobj_h__

#include "movableobj.h"
#include "rectangle.h"
#include "Projectile.h"

namespace Prototype
{
	class PlayerObj : public MovableObj
	{
	private:
		//size_t playerId;
		
		static const int N_WEAPONS = Projectile::N_TYPES;

		Projectile::Type currentWeapon; // used by client
		int ammo[N_WEAPONS]; // used by client
		int nextShootTime; // used by client
	public:

		static const float FORWARD_BACKWARD_SPEED;
		static const float STRAFE_SPEED;
		static const float RECTANGLE_SIZE;
		Pos pos;
		float angle;

		int health;

		bool movingForward;
		bool movingBackward;
		bool strafingLeft;
		bool strafingRight;

		//PlayerObj(size_t playerId, const Pos &pos);
		PlayerObj(const Pos &pos);

		Pos getPos() const			{ return pos; }
		void getRectangle(Rectangle &rectangle) const;
		//size_t getPlayerId() const		{ return playerId; }	

		void hurt(int damage);

		bool isDead()			{ return health <= 0; }

		void respawn(const Pos &respawnPos);

		// memberfunctions used by client
		inline Projectile::Type getCurrentWeapon() const	{return currentWeapon; }
		void setAmmoSupply(int seed);
		void switchWeapon();
		int getAmmoCurrentWeapon() const					{ return ammo[currentWeapon]; }
		inline bool canShoot(int time) const				{ return (ammo[currentWeapon] > 0) && (nextShootTime <= time); }
		void shoot(int time);

	};
};

#endif
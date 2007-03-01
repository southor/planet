#ifndef __playerobj_h__
#define __playerobj_h__

#include "MovableObj.h"
#include "Rectangle.h"
#include "Projectile.h"
#include "HistoryList.inl"
#include "Angle.h"
#include "messages.h"

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

		struct UpdateData
		{
			Pos pos;
			Angle angle;
			inline UpdateData()			{}
			inline UpdateData(const Pos &pos, Angle angle)
				: pos(pos), angle(angle)
			{}

			UpdateData operator +(const UpdateData &rh) const
			{
				UpdateData result(pos + rh.pos, angle + rh.angle);
				return result;
			}

			UpdateData operator -(const UpdateData &rh) const
			{
				UpdateData result(pos - rh.pos, angle - rh.angle);
				return result;
			}

			UpdateData operator *(float rh) const
			{
				UpdateData result(pos * rh, angle * rh);
				return result;
			}
		};
		
		HistoryList<UpdateData> historyList;

	public:

		static const float FORWARD_BACKWARD_SPEED;
		static const float STRAFE_SPEED;
		static const float RECTANGLE_SIZE;
		Pos pos;
		Angle angle;

		int health;

		bool movingForward;
		bool movingBackward;
		bool strafingLeft;
		bool strafingRight;

		//PlayerObj(size_t playerId, const Pos &pos);
		PlayerObj(const Pos &pos, size_t nHistoryTicks, int tick);

		~PlayerObj()										{}

		Pos getPos() const									{ return pos; }
		void getRectangle(Rectangle &rectangle) const;
		//size_t getPlayerId() const						{ return playerId; }	

		void hurt(int damage);

		bool isDead()										{ return health <= 0; }

		void respawn(const Pos &respawnPos);

		// memberfunctions used by client
		inline Projectile::Type getCurrentWeapon() const	{ return currentWeapon; }
		void setAmmoSupply(int seed);
		void switchWeapon();
		int getAmmoCurrentWeapon() const					{ return ammo[currentWeapon]; }
		inline bool canShoot(int time) const				{ return (ammo[currentWeapon] > 0) && (nextShootTime <= time); }
		void shoot(int time);

		inline void setTickData(int tick, const Pos &pos, Angle angle)
		{
			UpdateData data(pos, angle);
			historyList.setData(tick, data);
		}

		inline void storeToTickData(int tick)				{ setTickData(tick, pos, angle); }

		void updateToTickData(int tick);

		void updateToTickData(Tickf tick);

		inline int getLastStoredTick()						{ return historyList.getLastTick(); }

		void setUserCmd(const UserCmd *userCmd);

	};
};

#endif

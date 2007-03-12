#ifndef __playerobj_h__
#define __playerobj_h__

#include "MovableObj.h"
#include "Rectangle.h"
#include "Projectile.h"
#include "HistoryList.inl"
#include "Angle.h"
#include "messages.h"
//#include "UpdateData2.h"

namespace Prototype
{
	class PlayerObj : public MovableObj
	{
	private:
		//size_t playerId;
		
		static const int N_WEAPONS = Projectile::N_TYPES;

		//Projectile::Type currentWeapon;
		int ammo[N_WEAPONS];
		Tickf nextShootTick;

		//struct UpdateData : public UpdateData2<Pos, Angle>
		//{
		//	inline UpdateData()					{}
		//	inline UpdateData(const Pos &pos, Angle angle) : UpdateData2<Pos, Angle>(pos, angle)
		//	{}

		//	const Pos& getPos()		{ return v1; }
		//	Angle getAngle()		{ return v2; }
		//};
		struct UpdateData
		{

			Pos pos;
			Angle angle;
			Tickf nextShootTick;

			inline UpdateData()			{}
			inline UpdateData(const Pos &pos, Angle angle, Tickf nextShootTick)
				: pos(pos), angle(angle), nextShootTick(nextShootTick)
			{}

			UpdateData operator +(const UpdateData &rh) const
			{
				UpdateData result(pos + rh.pos, angle + rh.angle, nextShootTick + rh.nextShootTick);
				return result;
			}

			UpdateData operator -(const UpdateData &rh) const
			{
				UpdateData result(pos - rh.pos, angle - rh.angle, nextShootTick - rh.nextShootTick);
				return result;
			}

			UpdateData operator *(float rh) const
			{
				UpdateData result(pos * rh, angle * rh, nextShootTick * rh);
				return result;
			}

			bool operator !=(const UpdateData &rh) const
			{				
				return (pos != rh.pos) || (angle != rh.angle) || (nextShootTick != rh.nextShootTick);
			}
		};
		
		HistoryList<UpdateData> historyList;

		UserCmd userCmd;

	public:

		static const float FORWARD_BACKWARD_SPEED;
		static const float ROTATE_SPEED;
		static const float STRAFE_SPEED;
		static const float RECTANGLE_SIZE;
		Pos pos;
		Angle angle;

		

		int health;

		//bool movingForward;
		//bool movingBackward;
		//bool strafingLeft;
		//bool strafingRight;
		

		//PlayerObj(size_t playerId, const Pos &pos);
		PlayerObj(const Pos &pos, size_t nHistoryTicks, int tick);

		~PlayerObj()										{}

		Pos getPos() const									{ return pos; }
		//inline Angle getAngle()								{ return userCmd.aimAngle; }
		//inline void setAngle(Angle angle)					{ userCmd.aimAngle = angle; }
		void getRectangle(Rectangle &rectangle) const;
		//size_t getPlayerId() const						{ return playerId; }	

		void hurt(int damage);

		bool isDead()										{ return health <= 0; }

		void respawn(const Pos &respawnPos);


		Tickf getNextShootTick() const						{ return nextShootTick; }
		void setNextShootTick(Tickf nextShootTick)			{ this->nextShootTick = nextShootTick; }
		inline Projectile::Type getCurrentWeapon() const	{ return userCmd.weapon; }
		void setAmmoSupply(int seed);
		//void switchWeapon();
		Projectile::Type getNextWeapon(Projectile::Type weapon);

		const int* getAmmo() const							{ return ammo; }
		int getAmmoCurrentWeapon() const					{ return ammo[getCurrentWeapon()]; }
		//inline bool canShoot(int time) const				{ return (getAmmoCurrentWeapon() > 0) && (nextShootTick <= time); }
		//void shoot(int time);

		inline void setTickData(int tick, const Pos &pos, Angle angle, Tickf nextShootTick)
		{
			UpdateData data(pos, angle, nextShootTick);
			historyList.setData(tick, data);
		}

		// @return true if there was a differ
		bool setTickDataAndCompare(int tick, const Pos &pos, Angle angle, Tickf nextShootTick);

		inline void storeToTickData(int tick)				{ setTickData(tick, pos, angle, nextShootTick); }

		void updateToTickData(int tick);

		void updateToTickData(Tickf tick);

		inline int getLastStoredTick()						{ return historyList.getLastTick(); }

		const UserCmd& getUserCmd()							{ return userCmd; }
		void setUserCmd(const UserCmd *userCmd);

		int getNTickShots(Projectile::Type weapon, int currentTick, bool continuous);

		//static Projectile::Type switchWeapon(Projectile::Type currentWeapon)
		
		void updateNextShootTime(int currentTick);

		Tickf getShotTick(int currentTick, int shotN);

		bool isConsistent();

	};
};

#endif

#ifndef __playerobj_h__
#define __playerobj_h__

//#include "MovableObj.h"
//#include "Rectangle.h"
#include "Projectile.h"
#include "HistoryList.inl"
//#include "Angle.h"
#include "messages.h"
//#include "UpdateData2.h"
#include "Ship.h"

namespace Planet
{
	class PlayerObj
	{
	private:

		AmmoSupply ammoSupply;
		Tickf nextShootTick;
		
		struct UpdateData
		{

			Pos pos;
			Pos aimPos;
			Tickf nextShootTick;
			AmmoSupply ammoSupply;

			inline UpdateData()			{}
			inline UpdateData(const Pos &pos,
				const Pos &aimPos,
				Tickf nextShootTick, const AmmoSupply &ammoSupply
				)
				: pos(pos),
				  aimPos(aimPos),
				nextShootTick(nextShootTick),
				ammoSupply(ammoSupply)
			{
			}

			UpdateData operator +(const UpdateData &rh) const
			{
				UpdateData result(pos + rh.pos,
									aimPos + rh.aimPos,
									nextShootTick + rh.nextShootTick,
									ammoSupply
									);
				return result;
			}

			UpdateData operator -(const UpdateData &rh) const
			{
				UpdateData result(pos - rh.pos,
									aimPos - rh.aimPos,
									nextShootTick - rh.nextShootTick,
									ammoSupply
									);
				return result;
			}

			UpdateData operator *(float rh) const
			{
				UpdateData result(pos * rh,
									aimPos * rh,
									nextShootTick * rh,
									ammoSupply
									);
				return result;
			}

			bool operator !=(const UpdateData &rh) const
			{				
				return (pos != rh.pos) ||
						(aimPos != aimPos) ||
						(nextShootTick != rh.nextShootTick) || 
						(ammoSupply != ammoSupply);
			}

			static void interExtraPolate(int tick1, const UpdateData &data1, int tick2, const UpdateData &data2, Tickf resultTick, UpdateData &resultData);
	
			bool isConsistent() const
			{
				bool consistent = pos.length() < 100000.0f;
				assert(consistent);
				return consistent;
			}
		};
		

		HistoryList<UpdateData> historyList;

		UserCmd userCmd;

		static const float FORWARD_BACKWARD_SPEED; // speed in distance-units per millisecond
		static const float ROTATE_SPEED; // speed in angle-units per millisecond
		static const float STRAFE_SPEED; // speed in distance-units per millisecond

		inline void setTickData(int tick, const UpdateData &data)
		{
			historyList.setData(tick, data);
		}

	public:
		Ship ship;
		
		static const float RECTANGLE_SIZE;

		int health;

		PlayerObj(PlayerId playerId, const Color &playerColor, const Pos &pos, const Pos &aimPos, size_t nHistoryTicks, int tick, PlanetBody *planetBody);
		
		void tickInit(PlayerId playerId, int tick);

		~PlayerObj()										{}

		Ship* getShip()										{ return &ship; }

		static float getForwardBackwardSpeed()				{ return FORWARD_BACKWARD_SPEED * static_cast<float>(TimeHandler::TICK_DELTA_TIME); }
		static float getRotateSpeed()						{ return ROTATE_SPEED * static_cast<float>(TimeHandler::TICK_DELTA_TIME); }
		static float getStrafeSpeed()						{ return STRAFE_SPEED * static_cast<float>(TimeHandler::TICK_DELTA_TIME); }

		inline const Pos& getPos() const
		{
			assert(isConsistent());
			return ship.position;
		}

		inline const Pos& getAimPos() const						{ return ship.aimPos; }

		void hurt(int damage);

		bool isDead()										{ return health <= 0; }

		void respawn(const Pos &respawnPos);


		Tickf getNextShootTick() const						{ return nextShootTick; }
		void setNextShootTick(Tickf nextShootTick)			{ this->nextShootTick = nextShootTick; }
		inline Projectile::Type getCurrentWeapon() const	{ return userCmd.weapon; }
		void setAmmoSupply(int seed);
		Projectile::Type getNextWeapon(Projectile::Type weapon);

		const AmmoSupply& getAmmoSupply() const				{ return ammoSupply; }
		int getAmmoCurrentWeapon() const					{ return ammoSupply[getCurrentWeapon()]; }

		inline void setTickData(int tick, const UpdatePlayerObj *updatePlayerObj)			
		{
			UpdateData data(updatePlayerObj->pos, 
							updatePlayerObj->aimPos,
							updatePlayerObj->nextShootTick, updatePlayerObj->ammoSupply);
							setTickData(tick, data);
		}

		// @return true if there was a differ
		bool setTickDataAndCompare(int tick, const UpdatePlayerObj *updatePlayerObj);

		inline void storeToTickData(int tick)
		{
			assert(nextShootTick >= tick);				  
			UpdateData data(getPos(), getAimPos(), nextShootTick, ammoSupply);
			setTickData(tick, data);
		}

		void updateToTickData(int tick);

		void updateToTickData(Tickf tick);

		inline int getLastStoredTick()					{ return historyList.getLastTick(); }

		const UserCmd& getUserCmd()						{ return userCmd; }
		void setUserCmd(const UserCmd *userCmd);

		int getNTickShots(Projectile::Type weapon, int currentTick);

		// Updates nextShootTick for next tick (currentTick + 1)
		void updateNextShootTick(int currentTick);

		Tickf getShotTick(int currentTick, int shotN);

		bool isConsistent() const;
		bool isConsistent(int currentTick) const;

	};
};

#endif

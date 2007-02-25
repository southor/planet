#ifndef __projectile_h__
#define __projectile_h__

#include "MovableObj.h"
#include "Line.h"
#include "basic.h"
#include "HistoryList.inl"
#include "Angle.h"

namespace Prototype
{
	class Projectile : public MovableObj
	{
	private:

		HistoryList<Pos> historyList;

	public:

		struct Properties
		{
			float speed;
			int directDamage;
			int blastDamage;
			float blastDistance;
			int shootInterval;

			//Properties(float speed, int directDamage, int blastDamage, float blastDistance)
			//	: speed(speed), directDamage(directDamage), blastDamage(blastDamage), blastDistance(blastDistance)
			//{}
		};

		static const int N_TYPES = 2;
		typedef int Type;
		enum
		{			
			BULLET,
			ROCKET
		};

		Pos pos;

		Projectile(Type type, const Pos &pos, Angle angle, PlayerId shooterId, size_t nHistoryTicks);

		~Projectile()										{}

		inline Pos getPos() const							{ return pos; }
		inline void setPos(const Pos &pos)					{ this->pos = pos; }
		inline Type getType() const							{ return type; }
		inline Angle getAngle() const						{ return angle; }

		Line getLine() const;

		inline float getSpeed() const						{ return properties[type].speed; }
		inline int getDirectDamage() const					{ return properties[type].directDamage; }
		static inline int getDirectDamage(Type type)		{ return properties[type].directDamage; }
		static inline int getShootInterval(Type type)		{ return properties[type].shootInterval; }
		
		// @param blastPos The d-position along the line, 0.0 is beginning of line, 1.0 is end of line
		int getBlastDamage(float blastPos, const Pos &targetPos) const;
		
		
		PlayerId getShooterId() const						{ return shooterId; }

		inline void setUpdateData(int tick, const Pos &pos)	{ historyList.setData(tick, pos); }
		inline void storeToTickData(int tick)				{ historyList.setData(tick, pos); }
		inline void updateToTickData(int tick)				{ pos = historyList.getData(tick); }
		void updateToTickData(Tickf tick)					{ historyList.getData(tick, pos); }

	private:
		static const Properties properties[N_TYPES];

		Type type;
		Angle angle;
		PlayerId shooterId;
	};
};

#endif


#ifndef __projectile_h__
#define __projectile_h__

#include "movableobj.h"
#include "line.h"
#include "basic.h"

namespace Prototype
{
	class Projectile : public MovableObj
	{
	public:

		struct Properties
		{
			float speed;
			int directDamage;
			int blastDamage;
			float blastDistance;

			//Properties(float speed, int directDamage, int blastDamage, float blastDistance)
			//	: speed(speed), directDamage(directDamage), blastDamage(blastDamage), blastDistance(blastDistance)
			//{}
		};

		static const int N_TYPES = 2;
		enum Type
		{			
			BULLET,
			ROCKET
		};

		Pos pos;

		Projectile(Type type, const Pos &pos, float angle, size_t shooterId);

		inline Pos getPos() const					{ return pos; }
		inline Type getType() const					{ return type; }
		inline float getAngle() const				{ return angle; }

		Line getLine() const;

		inline float getSpeed() const				{ return properties[type].speed; }
		inline int getDirectDamage() const			{ return properties[type].directDamage; }
		//inline int getBlastDamage() const			{ return properties[type].blastDamage; }
		int getBlastDamage(const Pos &pos) const;
		//inline float getBlastDistance() const		{ return properties[type].blastDistance; }
		size_t getShooterId() const					{ return shooterId; }

	private:
		static const Properties properties[N_TYPES];

		Type type;
		float angle;
		size_t shooterId;
	};
};

#endif

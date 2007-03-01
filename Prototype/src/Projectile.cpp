#include "Projectile.h"
#include "TimeHandler.h"
#include <cmath>

namespace Prototype
{

	const Projectile::Properties Projectile::properties[2] = {{1000.0f/1000.0f, 20, 0, 0, 75},
															   {200.0f/1000.0f, 100, 50, 100.0f, 500}};

	Projectile::Projectile(Type type, const Pos &pos, Angle angle, PlayerId shooterId, size_t nHistoryTicks, int tick)
		: historyList(nHistoryTicks), type(type), pos(pos), angle(angle), shooterId(shooterId)
	{
		// insert data into history list
		historyList.setData(tick, pos);
	}

	Line Projectile::getLine() const
	{
		float length = TimeHandler::TICK_DELTA_TIME * getSpeed() * 1.2f;
		Vec direction(cos(angle.getFloat()), sin(angle.getFloat()));		
		Line line(pos, direction * length);
		return line;
	}

	int Projectile::getBlastDamage(float blastPos, const Pos &targetPos) const
	{
		assert(properties[type].blastDistance >= 0.0f);
		
		Pos blastPos2(getLine().getPosAlong(blastPos));
		float distance = tmax(abs(blastPos2.x - targetPos.x), abs(blastPos2.y - targetPos.y));		
		float blastDistance = properties[type].blastDistance;
		int blastDamage = 0;
		if (distance < blastDistance)
		{
			blastDamage = static_cast<int>(properties[type].blastDamage * (1.0f - distance / blastDistance)) + 1;
		}
		return blastDamage;
	}

};



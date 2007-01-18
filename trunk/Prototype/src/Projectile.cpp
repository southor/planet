#include "projectile.h"
#include "TimeHandler.h"
#include <cmath>

namespace Prototype
{

	const Projectile::Properties Projectile::properties[2] = {{1000.0f/1000.0f, 20, 0, 0},
															   {200.0f/1000.0f, 100, 50, 100.0f}};

	Projectile::Projectile(Type type, const Pos &pos, float angle, size_t shooterId)
		: type(type), pos(pos), angle(angle), shooterId(shooterId)
	{}

	Line Projectile::getLine() const
	{
		float length = TimeHandler::DELTA_TIME_MAX_F * getSpeed() * 1.2f;
		Vec direction(cos(angle), sin(angle));		
		Line line(pos, direction * length);
		return line;
	}

	int Projectile::getBlastDamage(const Pos &pos) const
	{
		float distance = tmax(abs(this->pos.x - pos.x), abs(this->pos.y - pos.y));		
		float blastDistance = properties[type].blastDistance;
		float blastDamage = 0.0f;
		if (distance < blastDistance)
		{
			blastDamage = properties[type].blastDamage * tmax(0.0f, (1.0f - (distance / blastDistance)));
		}
		return blastDamage;
	}

};


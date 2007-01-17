#include "projectile.h"
#include "TimeHandler.h"
#include <cmath>

namespace Prototype
{

	const Projectile::Properties Projectile::properties[2] = {{1000.0f/1000.0f, 20, 0, 0},
															   {200.0f/1000.0f, 100, 50, 100.0f}};

	Projectile::Projectile(Type type, const Pos &pos, float angle)
		: type(type), pos(pos), angle(angle)
	{}

	Line Projectile::getLine() const
	{
		float length = TimeHandler::DELTA_TIME_MAX_F * getSpeed() * 1.2f;
		Vec direction(cos(angle), sin(angle));		
		Line line(pos, direction * length);
		return line;
	}

};


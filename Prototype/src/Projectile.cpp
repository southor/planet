#include "Projectile.h"
#include "TimeHandler.h"
#include <cmath>

namespace Prototype
{

	const Projectile::Properties Projectile::properties[2] = {{200.0f/1000.0f, 20, 0, 0, 75},
															   {200.0f/1000.0f, 100, 50, 100.0f, 500}};

	Projectile::Projectile(Type type, const Pos &pos, Angle angle, GameObjId shooterId, size_t nHistoryTicks, int currentTick, Tickf shootTick, int objLag)
		: historyList(nHistoryTicks, getPosAtTick, this), type(type), pos(pos), angle(angle), shooterId(shooterId), shootTick(shootTick), objLag(objLag)
	{
		// Set position at currentTick, Projectiles current tick is actually shootTick before we set position
		this->pos = getPosAtTick(shootTick, currentTick);
		
		// insert data into history list
		historyList.setDefaultData(this->pos);
		historyList.setData(static_cast<int>(shootTick), pos);
	}

	Line Projectile::getLine() const
	{
		float length = getSpeed() * 1.2f;
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

	Pos Projectile::getPosAtTick(Tickf currentTick, Tickf resultTick)
	{
		Tickf tickDiff = resultTick - currentTick;
		float speed = getSpeed();
		Vec moveVec(getLine().getDirection() * speed * static_cast<float>(tickDiff));

		return getPos() + moveVec;
	}

	void Projectile::getPosAtTick(int tick, const Pos &pos, Tickf resultTick, Pos &resultPos, Projectile *projectile)
	{		
		Pos tmpPos(projectile->getPos());
		projectile->setPos(pos);

		resultPos = projectile->getPosAtTick(static_cast<Tickf>(tick), resultTick);

		projectile->setPos(tmpPos);
	}

};



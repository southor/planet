#ifndef __projectile_h__
#define __projectile_h__

#include "movableobj.h"

namespace Prototype
{
	class Projectile : public MovableObj, public Line
	{
	public:
		Pos getPos() const			{ return pos; }
	};
};

#endif

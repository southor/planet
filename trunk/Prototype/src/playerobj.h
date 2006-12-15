#ifndef __playerobj_h__
#define __playerobj_h__

#include "movableobj.h"

namespace Prototype
{
	class PlayerObj : public MovableObj, public Rectangle
	{
	public:
		Pos getPos() const			{ return pos; }
	};
};

#endif
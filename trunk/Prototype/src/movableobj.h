#ifndef __movableobj_h__
#define __movableobj_h__

#include "gameobj.h"

namespace Prototype
{
	class MovableObj : public GameObj
	{
	public:
		virtual Pos getPos() = 0;
	};
};

#endif
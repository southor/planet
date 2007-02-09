#ifndef __movableobj_h__
#define __movableobj_h__

#include "GameObj.h"
#include "basic.h"

namespace Prototype
{
	class MovableObj : public GameObj
	{
	public:
		virtual Pos getPos() const = 0;
	};
};

#endif
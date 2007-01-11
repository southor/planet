#ifndef __obstacle_h__
#define __obstacle_h__

#include "rectangle.h"
#include "gameobj.h"

namespace Prototype
{
	class Obstacle : public Rectangle, public GameObj
	{
	public:
		Obstacle(const Rectangle &area) : Rectangle(area)	{}
	};
};

#endif
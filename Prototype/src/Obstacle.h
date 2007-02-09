#ifndef __obstacle_h__
#define __obstacle_h__

#include "Rectangle.h"
#include "GameObj.h"

namespace Prototype
{
	class Obstacle : public Rectangle, public GameObj
	{
	public:
		Obstacle(const Rectangle &area) : Rectangle(area)	{}
	};
};

#endif
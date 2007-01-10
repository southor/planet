#ifndef __rectangle_cpp__
#define __rectangle_cpp__

#include "rectangle.h"

namespace Prototype
{
	bool Rectangle::overlapping(const Rectangle &rectangle)
	{
		return (rectangle.getLeft() < getRight()) &&
				(rectangle.getRight() > getLeft()) &&
				(rectangle.getTop() > getBottom()) &&
				(rectangle.getBottom() < getTop());
	}
};

#endif
#ifndef __rectangle_cpp__
#define __rectangle_cpp__

#include "rectangle.h"

namespace Prototype
{

	Rectangle::Rectangle(Pos centerPos, float size)
	{
		this->size.x = size;
		this->size.y = size;
		pos = centerPos - this->size / 2.0f;
	}

	bool Rectangle::overlapping(const Rectangle &rectangle)
	{
		return (rectangle.getLeft() < getRight()) &&
				(rectangle.getRight() > getLeft()) &&
				(rectangle.getTop() > getBottom()) &&
				(rectangle.getBottom() < getTop());
	}

	bool Rectangle::contains(const Pos &pos) const
	{
		return (pos.x > getLeft()) && (pos.x < getRight()) &&
				(pos.y > getBottom()) && (pos.y < getTop());
	}

	Line Rectangle::getLeftLine() const
	{		
		Vec ext(0.0f, size.y);
		return Line(getBottomLeft(), ext);
	}

	Line Rectangle::getRightLine() const
	{
		Vec ext(0.0f, size.y);
		return Line(getBottomRight(), ext);
	}

	Line Rectangle::getTopLine() const
	{
		Vec ext(size.x, 0.0f);
		return Line(getTopLeft(), ext);
	}

	Line Rectangle::getBottomLine() const
	{
		Vec ext(size.x, 0.0f);
		return Line(getBottomLeft(), ext);
	}

};

#endif
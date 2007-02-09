#ifndef __rectangle_cpp__
#define __rectangle_cpp__

#include "Rectangle.h"

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

	OrthogonalLine Rectangle::getLeftLine() const
	{
		return OrthogonalLine(OrthogonalLine::Y_AXIS, getBottomLeft(), size.y);
	}

	OrthogonalLine Rectangle::getRightLine() const
	{
		return OrthogonalLine(OrthogonalLine::Y_AXIS, getBottomRight(), size.y);
	}

	OrthogonalLine Rectangle::getTopLine() const
	{
		return OrthogonalLine(OrthogonalLine::X_AXIS, getTopLeft(), size.x);
	}

	OrthogonalLine Rectangle::getBottomLine() const
	{
		return OrthogonalLine(OrthogonalLine::X_AXIS, getBottomLeft(), size.x);
	}

};

#endif

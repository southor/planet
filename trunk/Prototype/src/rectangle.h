#ifndef __rectangle_h__
#define __rectangle_h__

#include "basic.h"

namespace Prototype
{
	class Rectangle
	{
	public:

		Pos pos;
		Vec size;

		inline Rectangle()														{}

		inline Rectangle(Pos pos, Vec size) : pos(pos), size(size)				{}

		inline Rectangle(float x, float y, float w, float h) : pos(x, y), size(w, h)	{}


		inline float getX() const								{ return pos.x; }
		inline float getY() const								{ return pos.y; }
		inline float getWidth() const							{ return size.x; }
		inline float getHeight() const							{ return size.y; }

		inline float getLeft() const							{ return pos.x; }
		inline float getRight() const							{ return pos.x + size.x; }
		inline float getBottom() const							{ return pos.y; }
		inline float getTop() const								{ return pos.y + size.y; }

		inline Pos getBottomRight() const						{ return Pos(getRight(), getBottom()); }
		inline Pos getTopRight() const							{ return Pos(getRight(), getTop()); }
		inline Pos getTopLeft() const							{ return Pos(getLeft(), getTop()); }		
		inline Pos getBottomLeft() const						{ return pos; }







	};
};

#endif
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


		//inline int getX() const								{ return pos.x; }
		//inline int getY() const								{ return pos.y; }
		//inline int getWidth() const							{ return size.x; }
		//inline int getHeight() const							{ return size.y; }

		//inline int getLeft() const							{ return pos.x; }
		//inline int getRight() const							{ return pos.x + size.x; }
		//inline int getBottom() const							{ return pos.y; }
		//inline int getTop() const								{ return pos.y + size.y; }

		//inline Pos getBottomRight() const						{ return Pos(getRight(), getBottom()); }
		//inline Pos getTopRight() const						{ return Pos(getRight(), getTop()); }
		//inline Pos getTopLeft() const							{ return Pos(getLeft(), getTop()); }		
		//inline Pos getBottomLeft() const						{ return pos; }







	};
};

#endif
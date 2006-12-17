#ifndef __line_h__
#define __line_h__

//#include <cmath>
#include "basic.h"

namespace Prototype
{
	class Line
	{
	public:

		Pos pos;
		Vec extension;

		inline Line()																{}

		inline Line(Pos pos, Vec extension) : pos(pos), extension(extension)		{}

		inline Line(float x, float y, float ex, float ey) : pos(x, y), extension(ex, ey)	{}
	};
};

#endif
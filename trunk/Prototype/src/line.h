#ifndef __line_h__
#define __line_h__

//#include <cmath>

namespace Prototype
{
	class Line
	{
	public:

		Pos pos;
		Vec extension;

		inline Line()																{}

		inline Line(Pos pos, Vec extension) : pos(pos), extension(extension)		{}

		inline Line(int x, int y, int ex, int ey) : pos(x, y), extension(ex, ey)	{}
	};
};

#endif
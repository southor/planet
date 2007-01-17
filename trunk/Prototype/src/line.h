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
		Vec ext;

		inline Line()																	{}

		inline Line(Pos pos, Vec ext) : pos(pos), ext(ext)								{}

		inline Line(float x, float y, float ex, float ey) : pos(x, y), ext(ex, ey)		{}

		// @return A normalized direction vector
		inline Vec getDirection()
		{
			Vec direction(ext);
			direction.normalize();
			return direction;
		}

		inline Pos getEndPos()
		{
			return pos+ext;
		}

		float crossPoint(const Line &line);

		inline bool crossing(const Line &line)
		{
			float d = crossPoint(line);
			return (d >= 0.0f) && (d <= 1.0f);
		}
	};
};

#endif
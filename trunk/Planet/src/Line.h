#ifndef __line_h__
#define __line_h__

#include "basic.h"


namespace Planet
{

	class Line
	{
	public:

		Pos pos;
		Vec ext;

		inline Line()																{}

		inline Line(Pos pos, Vec ext) : pos(pos), ext(ext)							{}

		inline Line(float x, float y, float z, float ex, float ey, float ez)
			: pos(x, y, z), ext(ex, ey, ez)											{}

		// @return A normalized direction vector
		inline Vec getDirection()
		{
			Vec direction(ext);
			direction.normalize();
			return direction;
		}

		inline Pos getEndPos() const						{ return pos+ext; }


		// @param d position along line
		inline static bool crossing(const float d)
		{
			return (d >= 0.0f) && (d <= 1.0f);
		}

		// @param d position along line
		inline Pos getPosAlong(float d)
		{
			return pos + ext * d;
		}

		const Line& operator=(const Line& line)
		{
			pos = line.pos;
			ext = line.ext;
			return *this;
		}
	};
};

#endif

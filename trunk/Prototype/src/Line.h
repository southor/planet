#ifndef __line_h__
#define __line_h__

//#include <cmath>
#include "OrthogonalLine.h"

namespace Prototype
{

	class Rectangle;

	class Line
	{
	public:

		Pos pos;
		Vec ext;

		inline Line()																	{}

		inline Line(Pos pos, Vec ext) : pos(pos), ext(ext)								{}

		inline Line(float x, float y, float ex, float ey) : pos(x, y), ext(ex, ey)		{}

		inline Line(OrthogonalLine orthogonalLine) : pos(orthogonalLine.pos), ext(orthogonalLine.getExtVec())
		{}

		// @return A normalized direction vector
		inline Vec getDirection()
		{
			Vec direction(ext);
			direction.normalize();
			return direction;
		}

		inline Pos getEndPos() const						{ return pos+ext; }

		bool rightSide(const Pos &pos);

		bool crossingRectangle(const Rectangle & rectangle);

		////@return position along line
		//float crossPoint(const Line &line);
		float crossPoint(OrthogonalLine line);

		// @return position along line
		float minCrossPoint(const Rectangle &rectangle);

		// @param d position along line
		inline static bool crossing(const float d)
		{
			return (d >= 0.0f) && (d <= 1.0f);
		}

		//inline bool crossing(const Line &line)
		//{
		//	float d = crossPoint(line);
		//	return crossing(d);			
		//}

		// @param d position along line
		inline Pos getPosAlong(float d)
		{
			return pos + ext * d;
		}
	};
};

#endif
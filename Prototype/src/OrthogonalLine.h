#ifndef __orthogonal_line_h__
#define __orthogonal_line_h__

//#include <cmath>
#include "basic.h"

namespace Prototype
{

	class OrthogonalLine
	{
	public:

		enum Axis
		{
			X_AXIS,
			Y_AXIS
		};

		Axis axis;
		Pos pos;
		float ext;

		inline OrthogonalLine()
		{}

		inline OrthogonalLine(Axis axis, Pos pos, float ext)
			: axis(axis), pos(pos), ext(ext)
		{}

		inline OrthogonalLine(Axis axis, float x, float y, float ext)
			: axis(axis), pos(x, y), ext(ext)
		{}

		inline Vec getExtVec() const						{ return  (axis == X_AXIS) ? Vec(ext, 0.0f) : Vec(0.0f, ext); }
		inline Pos getEndPos() const						{ return pos + getExtVec(); }

		// @param d position along line
		inline Pos getPosAlong(float d)
		{
			return pos + getExtVec() * d;
		}
	};
};

#endif
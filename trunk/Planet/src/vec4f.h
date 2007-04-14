#ifndef __vec4f_h__
#define __vec4f_h__

#include "Vec3f.h"

namespace Planet
{

	class Vec4f
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vec4f() 
		{
		}

		Vec4f(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w)	{}
		Vec4f(const Vec3f &v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w)		{}
	};

};

#endif


#ifndef __basic_h__
#define __basic_h__

//#include <vector>

#include "vec2.h"
#include "assert.h"
#include "vec3f.h"
#include "templatefunctions.h"
#include "macros.h"
//#include "mat3f.h"
//#include "SpherePoint.h"



namespace Planet
{
	const bool SERVER_PRINT_NETWORK_DEBUG = false;
	const bool CLIENT_PRINT_NETWORK_DEBUG = true;
	const int MAX_N_PLAYERS = 128;

	typedef unsigned char uchar;
	typedef unsigned char ushort;
	typedef unsigned int uint;	
	
	typedef Vec2<float> Vec2f;
	typedef Vec2<int> Vec2i;

	typedef Vec3f Vec;
	typedef Vec Pos;

	typedef float Tickf;

	//template <typename T>
	inline float radianToDegree(float angle)
	{
		return angle * 360.0f / (2.0f * PI_F);
	}
	

	typedef uint PlayerId;
};

#endif

#ifndef __basic_h__
#define __basic_h__

//#include <vector>

#include "vec2.h"
#include "assert.h"

#define PI 3.141592654f

namespace Prototype
{
	typedef Vec2<float> Vec2f;
	
	typedef Vec2f Vec;
	typedef Vec Pos;

	template <typename T>
	inline float radianToDegree(T angle)
	{
		return angle * 360.0f / (2.0f * PI);
	}

	//// Functor
	//template <class T>
	//class HasId
	//{
	//private:
	//	int id;
	//public:
	//	HasId(int id) : id(id)	{}
	//	bool operator ()(const T &o)	{ return o.id == this->id; }
	//	bool operator ()(const T *o)	{ return o->id == this->id; }
	//};



	
};

#endif
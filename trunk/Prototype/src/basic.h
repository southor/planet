#ifndef __basic_h__
#define __basic_h__

//#include <vector>

#include "vec2.h"
#include "assert.h"
#include "vec3f.h"
#include "mat3f.h"

#include <string>

#define PI_F 3.141592654f
#define TWO_PI_F 6.283185308f

namespace Prototype
{
	//debug
	static std::string nowRunning;


	const int MAX_N_PLAYERS = 128;
	
	typedef unsigned char uchar;
	typedef unsigned char ushort;
	typedef unsigned int uint;	
	
	typedef Vec2<float> Vec2f;
	
	typedef Vec2f Vec;
	typedef Vec Pos;

	typedef double Tickf;

	//template <typename T>
	inline float radianToDegree(float angle)
	{
		return angle * 360.0f / (2.0f * PI_F);
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

	template <class It, class Functor>
	void ForEach(It begin, It end, Functor &fun)
	{
		It &it=begin;
		for(; it!=end; ++it)
		{
			fun(*it);
		}
	}



	template <typename T>
	inline T tmin(T a, T b)			{ return a < b ? a : b; }

	template <typename T>
	inline T tmax(T a, T b)			{ return a > b ? a : b; }

	template <typename T>
	T tmin(T a, T b, T c, T d)
	{
		return tmin(tmin(a, b), tmin(c, d));		
	}


	//typedef size_t GameObjId;

	typedef uint PlayerId;


	//#define PRINT_VARIABLE (x)  std::cout << "x:"



	
};

#endif

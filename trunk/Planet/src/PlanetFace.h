#ifndef __PlanetFace_h__
#define __PlanetFace_h__

#include "common.h"
#include "basic.h"

namespace Planet
{
	class PlanetFace
	{
	public:
		PlanetFace(float r, Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4) 
			:	radius(r),
				resolution(20),
				initialized(false),
				v1(v1), 
				v2(v2), 
				v3(v3), 
				v4(v4), 
				sp1(v1), 
				sp2(v2), 
				sp3(v3), 
				sp4(v4),
				vertices(0),
				colors(0),
				indices(0)
		{
		}

		~PlanetFace()
		{
			delete [] vertices;
			delete [] colors;
			delete [] indices;
		}

		void init();

		void draw();

		float getHeight(float s, float t);

		// Checks intersection between the line sp-(0,0,0) and this plane
		bool findIntersection(SpherePoint &sp, float &s, float &t);

		// Checks intersection between the line p2-(0,0,0) and this plane
		bool findIntersection(Vec3f &p2, float &s, float &t);

		// Checks intersection between the line p2-p1 and this plane
		bool findIntersection(Vec3f &p1, Vec3f &p2, float &s, float &t);


		// Vertexes (corners) in clockwise order
		Vec3f v1;
		Vec3f v2;
		Vec3f v3;
		Vec3f v4;

		// Normal of this Face/Plane
		Vec3f N;

		SpherePoint sp1;
		SpherePoint sp2;
		SpherePoint sp3;
		SpherePoint sp4;
		
		float radius;

		int resolution;
		
		int numIndices;
		
		bool initialized;
		
		// vertex array
		Vec3f *vertices;
		
		// color array
		Vec3f *colors;

		// index array
		uint *indices;
	};
};


#endif
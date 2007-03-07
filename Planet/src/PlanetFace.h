#ifndef __PlanetFace_h__
#define __PlanetFace_h__

#include "common.h"
#include "basic.h"

namespace Planet
{
	class PlanetFace
	{
	public:
		PlanetFace() {}
		PlanetFace(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4) 
			:	v1(v1), 
				v2(v2), 
				v3(v3), 
				v4(v4), 
				sp1(v1), 
				sp2(v2), 
				sp3(v3), 
				sp4(v4)
		{
			midVector = (v1 + v2 + v3 + v4) / 4;
		}

		void draw();

		bool angleWithinFace(float phi, float theta);

		// Vertexes (corners) in clockwise order
		Vec3f v1;
		Vec3f v2;
		Vec3f v3;
		Vec3f v4;

		Vec3f midVector;

		SpherePoint sp1;
		SpherePoint sp2;
		SpherePoint sp3;
		SpherePoint sp4;
	};
};


#endif
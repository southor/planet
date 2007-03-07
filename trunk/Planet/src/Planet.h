#ifndef __Planet_h__
#define __Planet_h__

#include "common.h"
#include "basic.h"

#include "PlanetFace.h"

namespace Planet
{
	const float r = 5.0f;
	
	// the corners of the cube, clockwise.
	const Vec3f c1(r, r, r);
	const Vec3f c2(r, r, -r);
	const Vec3f c3(r, -r, -r);
	const Vec3f c4(r, -r, r);

	const Vec3f c5(-r, r, r);
	const Vec3f c6(-r, r, -r);
	const Vec3f c7(-r, -r, -r);
	const Vec3f c8(-r, -r, r);


	class Planet
	{
	public:
		Planet() :	viewAngle(0.0f), 
					viewAngle2(0.0f), 
					xFront(c1, c2, c3, c4),
					xBack(c6, c5, c8, c7),
					yFront(c5, c6, c2, c1),
					yBack(c4, c3, c7, c8),
					zFront(c5, c1, c4, c8),
					zBack(c2, c6, c7, c3)
		{
			printf("xFront: %d\n", xFront.angleWithinFace(0.0f, 0.0f));
			printf("xBack: %d\n", xBack.angleWithinFace(0.0f, 0.0f));
			printf("yFront: %d\n", yFront.angleWithinFace(0.0f, 0.0f));
			printf("yBack: %d\n", yBack.angleWithinFace(0.0f, 0.0f));
			printf("zFront: %d\n", zFront.angleWithinFace(0.0f, 0.0f));
			printf("zBack: %d\n", zBack.angleWithinFace(0.0f, 0.0f));
		}
	
		void render();
		
	
		float viewAngle;
		float viewAngle2;
		
	private:
		PlanetFace xFront;
		PlanetFace xBack;

		PlanetFace yFront;
		PlanetFace yBack;

		PlanetFace zFront;
		PlanetFace zBack;
	};
};

#endif

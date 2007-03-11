#ifndef __Planet_h__
#define __Planet_h__

#include <vector>

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

	typedef std::vector<PlanetFace*> Faces;


	class Planet
	{
	public:
		Planet() :	viewAngle(0.0f), 
					viewAngle2(0.0f),
					shipPhi(0.0f),
					shipTheta(0.0f),
					zoom(4.0f),
					runUp(false),
					runDown(false),
					runLeft(false),
					runRight(false),
					vsp(0.0f, 0.0f, 5.0f),
					xFront(r, c1, c2, c3, c4),
					xBack(r, c6, c5, c8, c7),
					yFront(r, c5, c6, c2, c1),
					yBack(r, c4, c3, c7, c8),
					zFront(r, c5, c1, c4, c8),
					zBack(r, c2, c6, c7, c3)
		{
			faces.push_back(&xFront);
			faces.push_back(&xBack);
			faces.push_back(&yFront);
			faces.push_back(&yBack);
			faces.push_back(&zFront);
			faces.push_back(&zBack);
		
			direction = (vsp + Vec3f(0.0f, 2.0f, 0.0f)) - vsp;
		
		/*
			printf("xFront: %d\n", xFront.angleWithinFace(0.0f, 0.0f));
			printf("xBack: %d\n", xBack.angleWithinFace(0.0f, 0.0f));
			printf("yFront: %d\n", yFront.angleWithinFace(0.0f, 0.0f));
			printf("yBack: %d\n", yBack.angleWithinFace(0.0f, 0.0f));
			printf("zFront: %d\n", zFront.angleWithinFace(0.0f, 0.0f));
			printf("zBack: %d\n", zBack.angleWithinFace(0.0f, 0.0f));
		*/
			/*
			Vec3f v(5.0f, 3.0f, 5.0f);
			float s, t;
			bool insideFace = xFront.findIntersection(v, s, t);
			if (insideFace)
				printf("INSIDE\n");
			else
				printf("OUTSIDE\n");
			printf("s: %f, t: %f\n", s, t);
			*/
		
		}
	
		void render();
		
		float getHeight(float phi, float theta);
	
		float viewAngle;
		float viewAngle2;
		
		float shipPhi;
		float shipTheta;
		
		bool runUp;
		bool runDown;
		bool runLeft;
		bool runRight;
		
		float zoom;
		
		Vec3f vsp;
		Vec3f direction;
		
	private:
		PlanetFace xFront;
		PlanetFace xBack;

		PlanetFace yFront;
		PlanetFace yBack;

		PlanetFace zFront;
		PlanetFace zBack;
		
		Faces faces;
	};
};

#endif

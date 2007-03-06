#ifndef __planet_h__
#define __planet_h__

#include "common.h"
#include "basic.h"

namespace Prototype
{
	class Face
	{
	public:
		Face() {}
		Face(Vec3f v1, Vec3f v2, Vec3f v3) 
			: v1(v1), v2(v2), v3(v3)	{}

		// Vertexes (corners) in clockwise order
		Vec3f v1;
		Vec3f v2;
		Vec3f v3;
		Vec3f v4;
	};

	const float r = 5.0f;
	// p - plus, m - minus
	const Vec3f xpypzp(r, r, r);
	const Vec3f xpypzm(r, r, -r);
	const Vec3f xpymzp(r, -r, r);
	const Vec3f xpymzm(r, -r, -r);

	const Vec3f xmypzp(-r, r, r);
	const Vec3f xmypzm(-r, r, -r);
	const Vec3f xmymzp(-r, -r, r);
	const Vec3f xmymzm(-r, -r, -r);


	class Planet
	{
	public:
		Planet() 
			: viewAngle(0.0f), 
				viewAngle2(0.0f), 
				xf(Vec3f(r, r, r), Vec3f(r, r, -r), Vec3f(r, -r, r)),
				zf(Vec3f(-r, r, r), Vec3f(r, r, r), Vec3f(-r, -r, r))
		{}
	
		void render();
		
	
		float viewAngle;
		float viewAngle2;


		Face xf; // +x face
		Face zf; // +z face
/*
			drawFace(Vec3f(-r, r, -r), Vec3f(-r, r, r), Vec3f(-r, -r, -r)); // -x face
			drawFace(Vec3f(r, r, -r), Vec3f(-r, r, -r), Vec3f(r, -r, -r)); // -z face
			
			drawFace(Vec3f(r, r, r), Vec3f(-r, r, r), Vec3f(r, r, -r)); // +y face
			drawFace(Vec3f(r, -r, r), Vec3f(-r, -r, r), Vec3f(r, -r, -r)); // -y face

*/
	private:

	};
};

#endif

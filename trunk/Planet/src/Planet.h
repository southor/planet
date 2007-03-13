#ifndef __Planet_h__
#define __Planet_h__

#include <vector>

#include "common.h"
#include "basic.h"

#include "PlanetFace.h"

namespace Planet
{
	typedef std::vector<PlanetFace*> Faces;

	class Planet
	{
	public:
		Planet(float radius);
	
		void render();
		
		void init();
		
		// Checks if line startPos - EndPos collides with planet
		// collisionPoint is set to point of collision or the closest point
		// returns if collision occured or not
		bool checkCollision(Vec3f startPos, Vec3f endPos, Vec3f *collisionPoint);

		// Returns the planet height at given angles.
		float getHeight(float phi, float theta);
	
	private:
		// planet radius
		float r;
	
		// the corners of the cube, clockwise.
		Vec3f c1;
		Vec3f c2;
		Vec3f c3;
		Vec3f c4;
		Vec3f c5;
		Vec3f c6;
		Vec3f c7;
		Vec3f c8;
	
		// the faces of the cube
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

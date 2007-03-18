#ifndef __PlanetBody_h__
#define __PlanetBody_h__

#include <vector>

#include "common.h"
#include "basic.h"

#include "PlanetFace.h"
#include "ConfigHandler.h"

namespace Planet
{
	typedef std::vector<PlanetFace*> Faces;

	class PlanetBody
	{
	public:
		PlanetBody(std::string map);
		~PlanetBody();
	
		void render();
		
		void init();
		
		// Checks if line startPos - EndPos collides with planet
		// collisionPoint is set to point of collision or the closest point
		// returns if collision occured or not
		bool checkCollision(Vec3f startPos, Vec3f endPos, Vec3f *collisionPoint);

		// Returns the planet height at given angles.
		float getHeight(float phi, float theta);
	
	private:
		// the corners of the cube, clockwise.
		Pos c1;
		Pos c2;
		Pos c3;
		Pos c4;
		Pos c5;
		Pos c6;
		Pos c7;
		Pos c8;
	
		// the faces of the cube
		PlanetFace *xFront;
		PlanetFace *xBack;

		PlanetFace *yFront;
		PlanetFace *yBack;

		PlanetFace *zFront;
		PlanetFace *zBack;
		
		Faces faces;
		
		ConfigHandler mapConfig;
	};
};

#endif

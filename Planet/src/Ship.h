#ifndef __Ship_h__
#define __Ship_h__

#include "common.h"
#include "basic.h"
#include "PlanetBody.h"

#define SHIP_ROTATE_LIMIT 40

namespace Planet
{
	class Ship
	{
	public:
		Ship() 
			:	position(0.0f, 0.0f, 6.29f),
				moveUp(false),
				moveDown(false),
				moveLeft(false),
				moveRight(false),
				rotateForward(0.0f),
				rotateLeft(0.0f),
				rotateForwardSmooth(0.0f),
				rotateLeftSmooth(0.0f)
		{
			reference = (position + Vec3f(0.0f, 2.0f, 0.0f)) - position;
		}
	
		void setPlanet(PlanetBody *planetBody) { this->planetBody = planetBody; }
	
		void logic();
		void lookAt(Vec3f &lookAt);
		
		
		void render();
		
	
		bool moveUp;
		bool moveDown;
		bool moveLeft;
		bool moveRight;
			
		Vec3f position;
		Vec3f direction;
		Vec3f reference;
		
	private:
		PlanetBody *planetBody;

		float rotateForward;
		float rotateLeft;

		float rotateForwardSmooth;
		float rotateLeftSmooth;
	};
};

#endif

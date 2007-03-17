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
				direction(0.0f, 0.0f, 0.0f),
				moveUp(false),
				moveDown(false),
				moveLeft(false),
				moveRight(false),
				rotateForward(0.0f),
				rotateLeft(0.0f),
				rotateForwardSmooth(0.0f),
				rotateLeftSmooth(0.0f)
		{
			setStartPosition(position);
		}
	
		void setStartPosition(Pos pos);
			
		void setPlanet(PlanetBody *planetBody) { this->planetBody = planetBody; }
	
		// Used by server/client
		void logic(Pos &lookAt);
		
		// Used by client
		void setState(Pos &pos, Vec &dir);
		
		// Used by client
		void render();
		
	
		bool moveUp;
		bool moveDown;
		bool moveLeft;
		bool moveRight;
			
		Pos position;
		Vec direction;
		Vec reference;
		
		Pos prevPosition;
		
	private:
		void updateRotation();
	
		PlanetBody *planetBody;

		// Used for visual rotation during redering
		float rotateForward;
		float rotateLeft;

		float rotateForwardSmooth;
		float rotateLeftSmooth;
	};
};

#endif

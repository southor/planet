#ifndef __Ship_h__
#define __Ship_h__

#include "common.h"
#include "basic.h"
#include "PlanetBody.h"
#include "messages.h"
#include "Cmds.h"

#define SHIP_ROTATE_LIMIT 40

namespace Planet
{
	class Ship
	{
	public:
		Ship(const Color &color, const Pos &pos, const Pos &aimPos, PlanetBody *planetBody) 
			:	color(color),
				position(pos),
				aimPos(aimPos),
				planetBody(planetBody),
				rotateForward(0.0f),
				rotateLeft(0.0f),
				rotateForwardSmooth(0.0f),
				rotateLeftSmooth(0.0f)
		{
			setReference(position);
		}
		
			
		void setPlanetBody(PlanetBody *planetBody) { this->planetBody = planetBody; }
	
		// Used by server/client
		void logic(UserCmd &userCmd);
		
		// Used by client
		void setState(Pos &pos, Pos &aimPos);
		
		// Used by client
		void render();
		
		Vec getNormal();
		Vec getDirection();
			
		Pos position;
		Pos aimPos;

		Vec reference;
		
		Pos prevPosition;
		
	private:
		void setReference(Pos pos);
		void updateRotation();
	
		PlanetBody *planetBody;
		
		Color color;

		// Used for visual rotation during redering
		float rotateForward;
		float rotateLeft;

		float rotateForwardSmooth;
		float rotateLeftSmooth;
	};
};

#endif

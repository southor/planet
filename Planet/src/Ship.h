#ifndef __Ship_h__
#define __Ship_h__

#include "common.h"
#include "basic.h"
#include "Planet.h"

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
				moveRight(false)
		{
			reference = (position + Vec3f(0.0f, 2.0f, 0.0f)) - position;
		}
	
		void setPlanet(Planet *planet) { this->planet = planet; }
	
		void logic();
		void render();
		
	
		bool moveUp;
		bool moveDown;
		bool moveLeft;
		bool moveRight;
			
		Vec3f position;
		Vec3f direction;
		Vec3f reference;
		
	private:
		Planet *planet;
	};
};

#endif

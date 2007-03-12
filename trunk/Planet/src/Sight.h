#ifndef __Sight_h__
#define __Sight_h__

#include "common.h"
#include "basic.h"
#include "Planet.h"
#include "Camera.h"


namespace Planet
{
	class Sight
	{
	public:
		Sight() : camera(0) {}
	
		void setCamera(Camera *camera) { this->camera = camera; }
		void setPlanet(Planet *planet) { this->planet = planet; }
	
		void update(Vec2<float> &mouseScreenPosRel);

		void render();
		
	private:
		Camera *camera;
		Planet *planet;
	public:
		Vec3f position;
	};
};

#endif

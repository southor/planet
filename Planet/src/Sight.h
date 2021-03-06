#ifndef __Sight_h__
#define __Sight_h__

#include "common.h"
#include "basic.h"
#include "PlanetBody.h"
#include "Camera.h"


namespace Planet
{
	class Sight
	{
	public:
		Sight();

		void init();
	
		void setCamera(Camera *camera) { this->camera = camera; }
		void setPlanetBody(PlanetBody *planetBody) { this->planetBody = planetBody; }
	
		void update(Vec2i &mouseScreenPos, int w, int h);

		void render();
		

		Vec3f position;

	private:
		Camera *camera;
		PlanetBody *planetBody;
		
		uint texture;
	};
};

#endif

#ifndef __Camera_h__
#define __Camera_h__

#include "common.h"
#include "basic.h"
#include "PlanetBody.h"


namespace Planet
{
	class Camera
	{
	public:
		Camera() : position(20.0f, 20.0f, 20.0f), up(0.0f, 1.0f, 0.0f), zoom(1.8f) {}
	
		void update(Vec3f &position, Vec3f &up);
		void useCamera();
	
		Vec3f position;
		Vec3f lookAt;
		Vec3f up;
		
		float zoom;
	};
};

#endif
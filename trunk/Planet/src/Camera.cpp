#include "Camera.h"

#include "SDL_OpenGL.h"

namespace Planet
{
	void Camera::update(Vec3f &position, Vec3f &up)
	{
		Vec3f down = -up;
		down.normalize();
		down = down / (zoom * 2.0f);
		
		Vec3f right = down.cross(position - position * this->zoom);
		right.normalize();
		right = right / zoom;
	
		this->position = position * this->zoom + down + right;
		this->lookAt = position;
		this->up = up;
	}

	void Camera::useCamera()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -1, 1, 1, 200);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
	}
};

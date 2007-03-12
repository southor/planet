#include "Camera.h"

#include "SDL_OpenGL.h"

namespace Planet
{
	void Camera::update(Vec3f &position, Vec3f &up)
	{
		this->position = position * this->zoom;
		this->lookAt = position;
		this->up = up;
	}

	void Camera::useCamera()
	{
		Vec3f look = lookAt - position;
	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-1, 1, -1, 1, 1, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(position.x, position.y, position.z, look.x, look.y, look.z, up.x, up.y, up.z);
	}
};

#include "Sight.h"

namespace Planet
{
	void Sight::update(Vec2i &mouseScreenPos, int w, int h)
	{
		Vec2f mouseScreenPosRel = Vec2f(
			mouseScreenPos.x / static_cast<float>(w), 
			mouseScreenPos.y / static_cast<float>(h));

		Vec3f look = camera->lookAt - camera->position;
	
		Vec3f viewPlaneVectorRight = look.cross(camera->up);
		viewPlaneVectorRight.normalize();

		Vec3f viewPlaneVectorUp = viewPlaneVectorRight.cross(look);
		viewPlaneVectorUp.normalize();
		
		Vec3f mouseViewPlanePos = camera->position 
			+ look/look.length() 
			+ viewPlaneVectorRight * (mouseScreenPosRel.x * 2.0f - 1.0f) 
			+ viewPlaneVectorUp * (mouseScreenPosRel.y * 2.0f - 1.0f);
		
		Vec3f mouseVector = mouseViewPlanePos - camera->position;

		bool mousePlanetCollision = planetBody->checkCollision(mouseViewPlanePos, mouseViewPlanePos + mouseVector * 1000.0f, &position);
	}
	
	void Sight::render()
	{
		glBegin(GL_POINTS);
			glColor3f(0.0, 1.0f, 0.0f);
			glVertex3f(position.x, position.y, position.z);
		glEnd();
	}
};
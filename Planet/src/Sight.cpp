#include "Sight.h"
#include "TextureHandler.h"

namespace Planet
{

	Sight::Sight() : camera(0), texture(0)
	{
	}
	
	void Sight::init() 
	{ 
		texture = TextureHandler::loadTexture("images/sight.png");
	}


	void Sight::update(Vec2i &mouseScreenPos, int w, int h)
	{
		Vec2f mouseScreenPosRel = Vec2f(
			mouseScreenPos.x / static_cast<float>(w), 
			mouseScreenPos.y / static_cast<float>(h));

		Vec look = camera->lookAt - camera->position;
	
		Vec viewPlaneVectorRight = look.cross(camera->up);
		viewPlaneVectorRight.normalize();

		Vec viewPlaneVectorUp = viewPlaneVectorRight.cross(look);
		viewPlaneVectorUp.normalize();

		viewPlaneVectorRight = viewPlaneVectorRight * (mouseScreenPosRel.x * 2.0f - 1.0f);
		viewPlaneVectorUp = viewPlaneVectorUp * (mouseScreenPosRel.y * 2.0f - 1.0f);

		// Give camera "mouse translate vector"
		camera->posTranslate = (viewPlaneVectorUp + viewPlaneVectorRight) * 3.0f;

		// Calculate mouse position in view plane
		Vec mouseViewPlanePos = (camera->position + camera->posTranslate)
			+ look/look.length() 
			+ viewPlaneVectorRight 
			+ viewPlaneVectorUp;
		
		Vec mouseVector = mouseViewPlanePos - (camera->position + camera->posTranslate);

		bool mousePlanetCollision = planetBody->checkCollision(mouseViewPlanePos, mouseViewPlanePos + mouseVector * 1000.0f, &position);
	}
	
	void Sight::render()
	{
		SpherePoint sp = position.toSpherePoint();
		
		VertData vertexData = planetBody->getVertexData(sp.phi, sp.theta);

		Vec normal = vertexData.planeNormal;

		Vec up = camera->up - ((camera->up).dot(normal) * normal);
		up.normalize();
		up = up / 5.0f;
		Vec left = up.cross(normal);

		Vec topLeft = position + up + left;
		Vec topRight = position + up - left;
		Vec bottomLeft = position - up + left;
		Vec bottomRight = position - up - left;
		
	
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(topLeft.x, topLeft.y, topLeft.z);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(topRight.x, topRight.y, topRight.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
		glEnd();
		/*
		glPointSize(10.0);
		glBegin(GL_POINTS);
			glVertex3f(position.x, position.y, position.z);
		glEnd();
		*/
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	}
};
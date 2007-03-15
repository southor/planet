#include "Ship.h"

namespace Planet
{
	void Ship::logic()
	{
		SpherePoint sp = position.toSpherePoint();
		sp.p = planetBody->getHeight(sp.phi, sp.theta);
		position = sp.toVector();
		
		
		Vec3f normal = position;
		normal.normalize();
		
		reference = reference - reference.dot(normal) * normal;

		Vec3f direction = reference;
		Vec3f directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction;
		Vec3f directionRight = -directionLeft;

		if (moveUp){
			position += direction / 20.0f;
			rotateForward = SHIP_ROTATE_LIMIT;
		}

		if (moveDown)
		{
			position -= direction / 20.0f;
			rotateForward = -SHIP_ROTATE_LIMIT;
		}

		if (!moveUp && !moveDown)
			rotateForward = 0.0f;
			
		if (moveLeft)
		{
			position += directionLeft / 20.0f;
			rotateLeft = SHIP_ROTATE_LIMIT;
		}
			
		if (moveRight)
		{
			position += directionRight / 20.0f;
			rotateLeft = -SHIP_ROTATE_LIMIT;
		}
		
		if (!moveLeft && !moveRight)
			rotateLeft = 0.0f;
			
		rotateForwardSmooth += (rotateForward - rotateForwardSmooth) / 10;
		rotateLeftSmooth += (rotateLeft - rotateLeftSmooth) / 10;
	}
	
	void Ship::lookAt(Vec3f &lookAt)
	{
		direction = lookAt - position;

		Vec3f normal = position;
		normal.normalize();
		
		Vec3f dn = direction.dot(normal) * normal;
		
		direction = direction - dn;
	}
	
	void Ship::render()
	{
		direction.normalize();
		direction = direction / 2.0f;
		
		Vec3f directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction;
		Vec3f directionRight = -directionLeft;

	
		glBegin(GL_LINES);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(position.x, position.y, position.z);
		glEnd();

		Vec3f p = position - direction / 2.0f;


		glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glRotatef(rotateForwardSmooth, directionLeft.x, directionLeft.y, directionLeft.z);
			glRotatef(-rotateLeftSmooth, direction.x, direction.y, direction.z);
			glTranslatef(-position.x, -position.y, -position.z);

			//glRotatef(rotateLeft, directionLeft.x, directionLeft.y, directionLeft.z);

			glBegin(GL_TRIANGLES);
				glColor3f(7.0f, 0.0f, 0.0f);
				glVertex3f(p.x + direction.x, p.y + direction.y, p.z + direction.z);
				glColor3f(0.2f, 0.2f, 0.2f);
				glVertex3f(p.x + directionLeft.x, p.y + directionLeft.y, p.z + directionLeft.z);
				glVertex3f(p.x + directionRight.x, p.y + directionRight.y, p.z + directionRight.z);
			glEnd();

			glPointSize(5.0);
			glBegin(GL_POINTS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(position.x, position.y, position.z);
			glEnd();

		glPopMatrix();
	}
};
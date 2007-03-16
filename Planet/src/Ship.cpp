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
		//direction = direction * 3.0f;
		Vec3f directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction;
		Vec3f directionRight = -directionLeft;

		prevPosition = position;


		if (moveUp)
			position += direction / 20.0f;

		if (moveDown)
			position -= direction / 20.0f;

		if (moveLeft)
			position += directionLeft / 20.0f;
			
		if (moveRight)
			position += directionRight / 20.0f;
	}
	
	void Ship::lookAt(Vec3f &lookAt)
	{
		direction = lookAt - position;

		Vec3f normal = position;
		normal.normalize();
		
		Vec3f dn = direction.dot(normal) * normal;
		
		direction = direction - dn;
	}
	
	void Ship::setState(Vec3f &pos, Vec3f &dir)
	{
		// Update position
		prevPosition = position;
		position = pos;		

		// Update direction
		direction = dir;
	}

	void Ship::updateRotation()
	{
		if (direction.length() != 0.0f)
			direction.normalize();

		// Rotate ship for display using prevPosition and position
		Vec3f movingVector = (position - prevPosition);
		
		Vec3f directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction;

		// Project movingVector onto direction to get rotation around directionLeft
		rotateForward = movingVector.dot(direction) * 300.0f;
		if (rotateForward > SHIP_ROTATE_LIMIT) rotateForward = SHIP_ROTATE_LIMIT;
		if (rotateForward < -SHIP_ROTATE_LIMIT) rotateForward = -SHIP_ROTATE_LIMIT;
		
		// Project movingVector onto directionLeft to get rotation around direction
		rotateLeft = movingVector.dot(directionLeft) * 300.0f;
		if (rotateLeft > SHIP_ROTATE_LIMIT) rotateLeft = SHIP_ROTATE_LIMIT;
		if (rotateLeft < -SHIP_ROTATE_LIMIT) rotateLeft = -SHIP_ROTATE_LIMIT;

		rotateForwardSmooth += (rotateForward - rotateForwardSmooth) / 10;
		rotateLeftSmooth += (rotateLeft - rotateLeftSmooth) / 10;
	}
		
	void Ship::render()
	{
		updateRotation();
	
		Vec3f normal = position;
		normal.normalize();

		direction.normalize();
		direction = direction / 2.0f;
		
		Vec3f directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction * 0.8f;
		Vec3f directionRight = -directionLeft;


		/*
		glBegin(GL_LINES);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(position.x, position.y, position.z);
		glEnd();
		*/
		
		Vec3f p = position - direction / 2.0f;
		Vec3f pTips = p - normal / 8.0f;

		glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glRotatef(rotateForwardSmooth, directionLeft.x, directionLeft.y, directionLeft.z);
			glRotatef(-rotateLeftSmooth, direction.x, direction.y, direction.z);
			glTranslatef(-position.x, -position.y, -position.z);

			// Left side
			glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(p.x + direction.x, p.y + direction.y, p.z + direction.z);
				glColor3f(0.1f, 0.1f, 0.1f);
				glVertex3f(pTips.x + directionLeft.x, pTips.y + directionLeft.y, pTips.z + directionLeft.z);
				glColor3f(0.8f, 0.1f, 0.0f);
				glVertex3f(p.x, p.y, p.z);
			glEnd();
			
			// Right side
			glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(p.x + direction.x, p.y + direction.y, p.z + direction.z);
				glColor3f(0.8f, 0.1f, 0.0f);
				glVertex3f(p.x, p.y, p.z);
				glColor3f(0.1f, 0.1f, 0.1f);
				glVertex3f(pTips.x + directionRight.x, pTips.y + directionRight.y, pTips.z + directionRight.z);
			glEnd();

			glPointSize(3.0);
			glBegin(GL_POINTS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(position.x, position.y, position.z);
			glEnd();

		glPopMatrix();
	}
};
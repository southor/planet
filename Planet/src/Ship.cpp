#include "Ship.h"

namespace Planet
{
	void Ship::setStartPosition(Pos pos)
	{
		reference = (position + position.getOrtoganal()) - position;
	}

	void Ship::logic(Pos &aimPos)
	{
		Vec normal = getNormal();
		
		// Set look direction 
		updateDirection(aimPos);

		// Update direction and position
		SpherePoint sp = position.toSpherePoint();
		sp.p = planetBody->getHeight(sp.phi, sp.theta);
		position = sp.toVector();
		
		reference = reference - reference.dot(normal) * normal;
		reference.normalize();

		Vec direction = reference;
		Vec directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction;
		Vec directionRight = -directionLeft;

		prevPosition = position;


		if (moveUp)
			position += direction / 10.0f;

		if (moveDown)
			position -= direction / 10.0f;

		if (moveLeft)
			position += directionLeft / 10.0f;
			
		if (moveRight)
			position += directionRight / 10.0f;
	}
	
	void Ship::setState(Pos &pos, Pos &aimPos)
	{
		// Update position
		prevPosition = position;
		position = pos;		

		// Update direction
		updateDirection(aimPos);

	}

	void Ship::updateDirection(Pos &aimPos)
	{
		Vec normal = getNormal();

		direction = aimPos - position;

		Vec dn = direction.dot(normal) * normal;
		
		direction = direction - dn;
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
	
		Vec normal = getNormal();

		direction.normalize();
		direction = direction / 2.0f;
		
		Vec directionLeft = Mat3f::rotateArbitrary(position, PI_F/2) * direction * 0.8f;
		Vec directionRight = -directionLeft;


		/*
		glBegin(GL_LINES);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(position.x, position.y, position.z);
		glEnd();
		*/
		
		Vec p = position - direction / 2.0f;
		Vec pTips = p - normal / 8.0f;

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

	Vec Ship::getNormal() 
	{ 
		Vec normal = position; 
		normal.normalize(); 
		return normal; 
	}
};
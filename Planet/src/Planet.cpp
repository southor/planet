#include "Planet.h"

namespace Planet
{
	void Planet::init()
	{
		xFront.init();
		xBack.init();

		yFront.init();
		yBack.init();

		zFront.init();
		zBack.init();
	}
		
	void Planet::render()
	{
		glLineWidth(2.0);
		glPointSize(3.0);

	/*
		glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(10.0f, 0.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 10.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 10.0f);
		glEnd();
	*/


		xFront.draw();
		xBack.draw();

		yFront.draw();
		yBack.draw();

		zFront.draw();
		zBack.draw();
	}
	
	bool Planet::checkCollision(Vec3f startPos, Vec3f endPos, Vec3f *collisionPos)
	{
		Vec3f direction = endPos - startPos;
		
		Vec3f vTest = startPos + direction / 2.0f;
		SpherePoint vTestSP = vTest.toSpherePoint();
		
		float planetHeight = getHeight(vTestSP.phi, vTestSP.theta);
		Vec3f planetPoint = SpherePoint(planetHeight, vTestSP.phi, vTestSP.theta).toVector();

		float heightDiff =  vTestSP.p - planetHeight;
		
		// Scalar should be negative so that we ignore the back side of the planet.
		float scalar = direction.dot(vTest);
		
		// Break condition
		if (std::abs(heightDiff) < 0.01f && scalar < 0)
		{
			*collisionPos = planetPoint;
			return true;
		}
			
		// Break condition
		if (direction.length() < 0.01f)
		{
			*collisionPos = planetPoint;
			return false;
		}
		
		if (heightDiff >= 0 &&  scalar < 0)
			return checkCollision(vTest, endPos, collisionPos);
		else
			return checkCollision(startPos, vTest, collisionPos);		
	}

	
	float Planet::getHeight(float phi, float theta)
	{
		float s, t;
		SpherePoint sp(1.0f, phi, theta);
		
		for (Faces::iterator it = faces.begin(); it < faces.end(); it++)
		{
			PlanetFace *face = *it;

			if (face->findIntersection(sp, s, t))
			{
				return face->getHeight(s, t);
			}
		}
		
		assert(true);
	}	
};

#include "Planet.h"

namespace Planet
{
	Planet::Planet(float radius) 
		:	r(radius),

			// the corners of the cube, clockwise.
			c1(r, r, r),
			c2(r, r, -r),
			c3(r, -r, -r),
			c4(r, -r, r),
			c5(-r, r, r),
			c6(-r, r, -r),
			c7(-r, -r, -r),
			c8(-r, -r, r),

			// the faces of the cube
			xFront(r, c1, c2, c3, c4, "ht.png", "grass_texture.png"),
			xBack(r, c6, c5, c8, c7, "ht.png", "grass_snow_texture.jpg"),
			yFront(r, c5, c6, c2, c1, "ht.png", "grass_texture.png"),
			yBack(r, c4, c3, c7, c8, "ht.png", "grass_texture.png"),
			zFront(r, c5, c1, c4, c8, "ht.png", "grass_texture.png"),
			zBack(r, c2, c6, c7, c3, "ht.png", "grass_texture.png")
	{
		faces.push_back(&xFront);
		faces.push_back(&xBack);
		faces.push_back(&yFront);
		faces.push_back(&yBack);
		faces.push_back(&zFront);
		faces.push_back(&zBack);
	}


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
		/*
		glLineWidth(2.0);
		glPointSize(3.0);

		glDisable(GL_LIGHTING);
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
		glEnable(GL_LIGHTING);
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
		return 0.0f;
	}	
};

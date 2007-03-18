#include "PlanetBody.h"

#include "ConfigHandler.h"

#include <string>

namespace Planet
{
	PlanetBody::PlanetBody() : xFront(0), xBack(0), yFront(0), yBack(0), zFront(0), zBack(0)
	{
	}

	PlanetBody::~PlanetBody()
	{
		delete xFront; delete xBack;
		delete yFront; delete yBack;
		delete zFront; delete zBack;
	}

	void PlanetBody::init(const std::string &map)
	{
		// Read map config file
		mapConfig.loadFile(map + "config.txt");

		float r = mapConfig.getFloatValue("radius");
		float detailScale = mapConfig.getFloatValue("detailScale");
		float heightMapMulti = mapConfig.getFloatValue("heightMapMulti");

		// the corners of the cube, clockwise.
		c1 = Pos(r, r, r),
		c2 = Pos(r, r, -r),
		c3 = Pos(r, -r, -r),
		c4 = Pos(r, -r, r),
		c5 = Pos(-r, r, r),
		c6 = Pos(-r, r, -r),
		c7 = Pos(-r, -r, -r),
		c8 = Pos(-r, -r, r),

		// the faces of the cube
		xFront = new PlanetFace(r, detailScale, heightMapMulti, c1, c2, c3, c4, map + "heightmap_xfront.png", map + "texture_xfront.png", map + "detail.png");
		xBack = new PlanetFace(r, detailScale, heightMapMulti, c6, c5, c8, c7, map + "heightmap_xback.png", map + "texture_xback.png", map + "detail.png");
		yFront = new PlanetFace(r, detailScale, heightMapMulti, c5, c6, c2, c1, map + "heightmap_yfront.png", map + "texture_yfront.png", map + "detail.png");
		yBack = new PlanetFace(r, detailScale, heightMapMulti, c4, c3, c7, c8, map + "heightmap_yback.png", map + "texture_yback.png", map + "detail.png");
		zFront = new PlanetFace(r, detailScale, heightMapMulti, c5, c1, c4, c8, map + "heightmap_zfront.png", map + "texture_zfront.png", map + "detail.png");
		zBack = new PlanetFace(r, detailScale, heightMapMulti, c2, c6, c7, c3, map + "heightmap_zback.png", map + "texture_zback.png", map + "detail.png");

		faces.push_back(xFront);
		faces.push_back(xBack);
		faces.push_back(yFront);
		faces.push_back(yBack);
		faces.push_back(zFront);
		faces.push_back(zBack);

		xFront->init();
		xBack->init();

		yFront->init();
		yBack->init();

		zFront->init();
		zBack->init();
	}
		
	void PlanetBody::render()
	{
		
		glLineWidth(2.0);
		glPointSize(3.0);

		/*
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

		
		xFront->render();
		xBack->render();

		yFront->render();
		yBack->render();

		zFront->render();
		zBack->render();
		
	}
	
	bool PlanetBody::checkCollision(Vec3f startPos, Vec3f endPos, Vec3f *collisionPos)
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

	
	float PlanetBody::getHeight(float phi, float theta)
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

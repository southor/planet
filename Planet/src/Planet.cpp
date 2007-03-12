#include "Planet.h"
#include "SDL_OpenGL.h"


namespace Planet
{
	void Planet::render()
	{
		xFront.init();
		xBack.init();

		yFront.init();
		yBack.init();

		zFront.init();
		zBack.init();

	
		//float height = getHeight(shipPhi, shipTheta);
		
		//static Vec3f vsp(0.0f, 0.0f, height);
		//static Vec3f direction = (vsp + Vec3f(0.0f, 2.0f, 0.0f)) - vsp;

		SpherePoint sp = vsp.toSpherePoint();
		sp.p = getHeight(sp.phi, sp.theta);
		vsp = sp.toVector();
		
		Vec3f normal = vsp;
		normal.normalize();
		
		direction = direction - direction.dot(normal) * normal;

		Vec3f directionLeft = Mat3f::rotateArbitrary(vsp, PI_F/2) * direction;
		Vec3f directionRight = -directionLeft;

		if (runUp)
			vsp += direction / 10.0f;
		if (runDown)
			vsp -= direction / 10.0f;
		if (runLeft)
			vsp += directionLeft / 10.0f;
		if (runRight)
			vsp += directionRight / 10.0f;
	
	
	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45.0f, 1.3333f, 0.1f, 100.0f);
		glFrustum(-1, 1, -1, 1, 1, 100);
		
		Vec3f cameraPos(vsp.x*zoom, vsp.y*zoom, vsp.z*zoom);
		Vec3f cameraLook = vsp - cameraPos;
		Vec3f cameraUp = direction;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraLook.x, cameraLook.y, cameraLook.z, cameraUp.x, cameraUp.y, cameraUp.z);

		//gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);

		

		Vec3f viewPlaneVectorRight = cameraLook.cross(cameraUp);
		viewPlaneVectorRight.normalize();

		Vec3f viewPlaneVectorUp = viewPlaneVectorRight.cross(cameraLook);
		viewPlaneVectorUp.normalize();
		
		Vec3f mouseViewPlanePos = cameraPos 
			+ cameraLook/cameraLook.length() 
			+ viewPlaneVectorRight * (mouseScreenPosRel.x * 2.0f - 1.0f) 
			+ viewPlaneVectorUp * (mouseScreenPosRel.y * 2.0f - 1.0f);
		
		Vec3f mouseVector = mouseViewPlanePos - cameraPos;

		Vec3f collisionPoint;
		bool mousePlanetCollision = checkCollision(mouseViewPlanePos, mouseViewPlanePos + mouseVector * 1000.0f, &collisionPoint);


		glLineWidth(2.0);
		glPointSize(3.0);

		glDisable(GL_LIGHTING);
		glPushMatrix();

			glRotatef(viewAngle, 0.0f, 1.0f, 0.0f);
			glRotatef(viewAngle2, 1.0f, 0.0f, 0.0f);

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
			
			


			
			glBegin(GL_LINES);
				glColor3f(0.5f, 0.5f, 0.5f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(vsp.x, vsp.y, vsp.z);
			glEnd();

			glPointSize(10.0);
			glBegin(GL_POINTS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(vsp.x, vsp.y, vsp.z);
			glEnd();

			glBegin(GL_LINES);
				glVertex3f(vsp.x, vsp.y, vsp.z);
				glVertex3f(vsp.x + direction.x, vsp.y + direction.y, vsp.z + direction.z);
			glEnd();

			/*
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(vsp.x, vsp.y, vsp.z);
				glVertex3f(vsp.x + directionLeft.x, vsp.y + directionLeft.y, vsp.z + directionLeft.z);
			glEnd();
			glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(vsp.x, vsp.y, vsp.z);
				glVertex3f(vsp.x + directionRight.x, vsp.y + directionRight.y, vsp.z + directionRight.z);
			glEnd();
			*/
			/*
			for (int i = 1; i < 100; i++)
			{
				float ii = static_cast<float>(i);
				glBegin(GL_POINTS);
					glColor3f(1.0, 0.0f, 0.0f);
					glVertex3f(mouseViewPlanePos.x+mouseVector.x*ii, mouseViewPlanePos.y+mouseVector.y*ii, mouseViewPlanePos.z+mouseVector.z*ii);
				glEnd();
			}
			*/
/*
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(mouseViewPlanePos.x, mouseViewPlanePos.y, mouseViewPlanePos.z);
				glVertex3f(mouseViewPlanePos.x+mouseVector.x*100.0f, mouseViewPlanePos.y+mouseVector.y*100.0f, mouseViewPlanePos.z+mouseVector.z*40.0f);
			glEnd();
*/

				
			if (mousePlanetCollision)
			{
				glBegin(GL_POINTS);
					glColor3f(0.0, 1.0f, 0.0f);
					glVertex3f(collisionPoint.x, collisionPoint.y, collisionPoint.z);
				glEnd();

				glBegin(GL_LINES);
					glColor3f(0.0, 0.5f, 0.0f);
					glVertex3f(vsp.x, vsp.y, vsp.z);
					glVertex3f(collisionPoint.x, collisionPoint.y, collisionPoint.z);
				glEnd();

			}





		glPopMatrix();
		glEnable(GL_LIGHTING);
	
	}
	
	bool Planet::checkCollision(Vec3f startPos, Vec3f endPos, Vec3f *collisionPos)
	{
		Vec3f direction = endPos - startPos;
		
		Vec3f vTest = startPos + direction / 2.0f;
		SpherePoint vTestSP = vTest.toSpherePoint();
		
		float heightDiff =  vTestSP.p - getHeight(vTestSP.phi, vTestSP.theta);
		
		// Scalar should be negative so that we ignore the back side of the planet.
		float scalar = direction.dot(vTest);
		
		// Break condition
		if (std::abs(heightDiff) < 0.01f && scalar < 0)
		{
			*collisionPos = vTest;
			return true;
		}
			
		// Break condition
		if (direction.length() < 0.01f)
			return false;
		
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

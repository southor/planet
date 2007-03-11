#include "Planet.h"
#include "SDL_OpenGL.h"


namespace Planet
{
	void Planet::render()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(45.0f, 1.3333f, 0.1f, 100.0f);
		glFrustum(-1, 1, -1, 1, 1, 100);
		
		Vec3f camera(vsp.x*zoom, vsp.y*zoom, vsp.z*zoom);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera.x, camera.y, camera.z, 0.0, 0.0, 0.0, direction.x, direction.y, direction.z);


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
			xFront.init();
			xBack.init();

			yFront.init();
			yBack.init();

			zFront.init();
			zBack.init();



			xFront.draw();
			
			xBack.draw();

			yFront.draw();
			yBack.draw();

			zFront.draw();
			
			zBack.draw();
			
			
			float height = getHeight(shipPhi, shipTheta);
			
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


			for (int i = 1; i < 20; i++)
			{
				float ii = static_cast<float>(i);
				glBegin(GL_POINTS);
					glColor3f(0.0f, 0.0f, 1.0f);
					glVertex3f(vsp.x*4.0f/ii, vsp.y*4.0f/ii, vsp.z*4.0f/ii);
				glEnd();
			}


		glPopMatrix();
		glEnable(GL_LIGHTING);
	
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

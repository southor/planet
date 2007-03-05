#include "Planet.h"
#include "SDL_OpenGL.h"


namespace Prototype
{
	void drawFace(Vec3f v1, Vec3f v2, Vec3f v3);

	void Planet::render()
	{
		float r = 5.0f;
		
		glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
		glLoadIdentity();  // Reset The Projection Matrix
		gluPerspective(45.0f, 1.3333f, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);  // Select The Modelview Matrix
		glLoadIdentity();  // Reset The Modelview Matrix
		gluLookAt(20.0, 10.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


		glLineWidth(2.0);
		glPointSize(3.0);


		glDisable(GL_LIGHTING);
		
		glDisable(GL_LIGHTING);
		glPushMatrix();
			glRotatef(viewAngle, 0.0f, 1.0f, 0.0f);
			glRotatef(viewAngle2, 1.0f, 0.0f, 0.0f);
		
		
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


			//glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
			//glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
			 
			float p;
			float phi;
			float theta;
			
			// a represents the length from center to the corner of the
			// planets "height map box". (2a)^2 = (2p)^2 + (2p)^2 + (2p)^2 (pythagorean theorem for box)
			float a = sqrt(3) * r;
								
			Vec3f v(0.0, 0.0, r);
			
			// get phi and theta from v 
			//CartesianToSpherical(v, &p, &phi, &theta);
			
			// "rotate" to get the corner points
			
			//Vec3f v1 = SphericalToCartesian(a, phi, theta);

			drawFace(Vec3f(r, r, r), Vec3f(r, r, -r), Vec3f(r, -r, r)); // +x face
			drawFace(Vec3f(-r, r, r), Vec3f(r, r, r), Vec3f(-r, -r, r)); // +z face

			drawFace(Vec3f(-r, r, -r), Vec3f(-r, r, r), Vec3f(-r, -r, -r)); // -x face
			drawFace(Vec3f(r, r, -r), Vec3f(-r, r, -r), Vec3f(r, -r, -r)); // -z face
			
			drawFace(Vec3f(r, r, r), Vec3f(-r, r, r), Vec3f(r, r, -r)); // +y face
			drawFace(Vec3f(r, -r, r), Vec3f(-r, -r, r), Vec3f(r, -r, -r)); // -y face
			
			
			//Vec3f v1 = SphericalToCartesian(a, phi-PI_F/4, theta+PI_F/4);
			//Vec3f v2 = SphericalToCartesian(a, phi+PI_F/4, theta+PI_F/4);
					//Vec3f v3 = SphericalToCartesian(a, phi+PI_F/4, theta-PI_F/4);
			//Vec3f v3 = SphericalToCartesian(a, phi-PI_F/4, theta-PI_F/4);


		glPopMatrix();
		glEnable(GL_LIGHTING);
	
	}

	void drawFace(Vec3f v1, Vec3f v2, Vec3f v3)
	{
		Vec3f v1v2 = v2 - v1;
		Vec3f v1v3 = v3 - v1;

		float time = SDL_GetTicks();

		float p;
		float phi;
		float theta;

		const int DOTS = 20;

		for (int i = 0; i <= DOTS; i++)
		{
			for (int j = 0; j <= DOTS; j++)
			{
				float ii = i/static_cast<float>(DOTS);
				float jj = j/static_cast<float>(DOTS);
	
				Vec3f v = v1 + v1v2*ii + v1v3*jj;
/*
				v.x = r;
				v.y = v1.y - i;
				v.z = v1.z - j;
*/
				CartesianToSpherical(v, &p, &phi, &theta);
				p = 5.0f + sin(i/5.0f + time/200.0f);
				Vec3f vSphere = SphericalToCartesian(p, phi, theta);

				glPushMatrix();

					glBegin(GL_POINTS);
						glColor3f(0.8f, 0.8f, 0.8f);
						//glVertex3f(v.x, v.y, v.z);

						glColor3f(v.x/7.0f, v.y/7.0f, v.z/7.0f);
						glVertex3f(vSphere.x, vSphere.y, vSphere.z);
					glEnd();
				
				glPopMatrix();
			}
		}

	}
	
};

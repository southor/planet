#include "Planet.h"
#include "SDL_OpenGL.h"


namespace Planet
{
	void drawFace(Vec3f v1, Vec3f v2, Vec3f v3);

	void Planet::render()
	{
		//float r = 5.0f;
		
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
			float a = sqrt(3.0f) * r;
								
			Vec3f v(0.0, 0.0, r);
			
			// get phi and theta from v 
			//CartesianToSpherical(v, &p, &phi, &theta);
			
			// "rotate" to get the corner points
			
			//Vec3f v1 = SphericalToCartesian(a, phi, theta);

			xFront.draw();
			xBack.draw();

			yFront.draw();
			yBack.draw();

			zFront.draw();
			zBack.draw();

/*
			drawFace(Vec3f(r, r, r), Vec3f(r, r, -r), Vec3f(r, -r, r)); // +x face
			drawFace(Vec3f(-r, r, r), Vec3f(r, r, r), Vec3f(-r, -r, r)); // +z face

			drawFace(Vec3f(-r, r, -r), Vec3f(-r, r, r), Vec3f(-r, -r, -r)); // -x face
			drawFace(Vec3f(r, r, -r), Vec3f(-r, r, -r), Vec3f(r, -r, -r)); // -z face
			
			drawFace(Vec3f(r, r, r), Vec3f(-r, r, r), Vec3f(r, r, -r)); // +y face
			drawFace(Vec3f(r, -r, r), Vec3f(-r, -r, r), Vec3f(r, -r, -r)); // -y face
*/			
			
			//Vec3f v1 = SphericalToCartesian(a, phi-PI_F/4, theta+PI_F/4);
			//Vec3f v2 = SphericalToCartesian(a, phi+PI_F/4, theta+PI_F/4);
					//Vec3f v3 = SphericalToCartesian(a, phi+PI_F/4, theta-PI_F/4);
			//Vec3f v3 = SphericalToCartesian(a, phi-PI_F/4, theta-PI_F/4);


		glPopMatrix();
		glEnable(GL_LIGHTING);
	
	}
};

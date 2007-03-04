#include "Planet.h"
#include "SDL_OpenGL.h"


namespace Prototype
{

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
			 
			 
			 

			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					float a = i / 5.0f;
					float b = j / 5.0f;
					/*
					float w = 0.0f;
					float ii = j/6.0f;
					float Omega = i/6.0f;
					*/
					
					Vector3 v(r, 0.0, 0.0);

					//Mat3f mor(Mat3f::rotateZ(zAngle));
					//Mat3f ir(Mat3f::rotateX(yAngle));
					//Mat3f wr(Mat3f::rotateZ(zAngle));
					
					//Mat3f m(ir * wr);

					//v = m * v;
					
					a += 45.0f;
					b -= 45.0f;


					v.x = r * sin(a) * cos(b);
					v.y = r * sin(a) * sin(b);
					v.z = r * cos(a);



					glPushMatrix();

					//glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
					//glRotatef(-zAngle, 0.0f, 0.0f, 1.0f);

					glBegin(GL_POINTS);
						glColor3f(j/10.0f, 0.0f, 0.0f);
						//glVertex3f(r, 0.0f, 0.0f);
						glVertex3f(v.x, v.y, v.z);
					glEnd();
					
					glPopMatrix();
				}
			}

		glPopMatrix();
		glEnable(GL_LIGHTING);
	
	}

};

#include "PlanetFace.h"

namespace Planet
{
	void PlanetFace::draw()
	{

		Vec3f v1v2 = v2 - v1;
		Vec3f v1v4 = v4 - v1;

		const int DOTS = 20;

		for (int i = 0; i <= DOTS; i++)
		{
			for (int j = 0; j <= DOTS; j++)
			{
				float ii = i/static_cast<float>(DOTS);
				float jj = j/static_cast<float>(DOTS);
	
				Vec3f v = v1 + v1v2*ii + v1v4*jj;
/*
				CartesianToSpherical(v, &p, &phi, &theta);
				p = 5.0f; // + sin(i/5.0f + SDL_GetTicks()/200.0f);
				Vec3f vSphere = SphericalToCartesian(p, phi, theta);
*/
				SpherePoint sp = v.toSpherePoint();
				sp.p = 5.0f;// + sin(i/5.0f + SDL_GetTicks()/200.0f);
				Vec3f vSphere = sp.toVector();


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
	
	bool PlanetFace::angleWithinFace(float phi, float theta)
	{
		/*
		SpherePoint sp(r, phi, theta);
		Vec3f v = sp.toVector();
		
		Vec3f distV = midVector - sp.toVector();
		*/
		return false;
	}
	

};

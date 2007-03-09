#include "PlanetFace.h"

namespace Planet
{
	void PlanetFace::init()
	{
		// create arrays
		numIndices = resolution*resolution*2 - 2*resolution;
		
		vertices = new Vec3f[resolution*resolution];
		colors = new Vec3f[resolution*resolution];
		indices = new uint[numIndices];
	
		Vec3f v1v2 = v2 - v1;
		Vec3f v1v4 = v4 - v1;
		N = v1v2.cross(v1v4);

		// Fill up vertices and colors arrays
		for (int i = 0; i < resolution; i++)
		{
			for (int j = 0; j < resolution; j++)
			{
				int index = i * resolution + j;

				// s and t will be between 0.0 and 1.0
				float s = j/static_cast<float>(resolution-1);
				float t = i/static_cast<float>(resolution-1);
	
				Vec3f v = v1 + v1v2*s + v1v4*t;
				
				// Convert to spherepoint, set height and then back
				SpherePoint sp = v.toSpherePoint();
				sp.p = getHeight(s, t);
				Vec3f vSphere = sp.toVector();
	
				vertices[index] = vSphere;
				colors[index] = Vec3f(0.0, (sp.p-4.0f)/3.0f, 0.0);
			}
		}
		
		int index = 0;
		
		// Fill up indices array
		for (int i = 0; i < resolution - 1; i++)
		{
			for (int j = 0; j < resolution; j++)
			{
				int currentIndex = 0;
				int nextIndex = 0;
			
				// If even row
				if (i % 2 == 0)
				{
					currentIndex	= i * resolution + j;
					nextIndex		= (i + 1) * resolution + j;
				}
				else
				{
					// Traverse from right to left
					currentIndex	= (i + 1) * resolution + (resolution - j - 1);
					nextIndex		= i * resolution + (resolution - j - 1);
				}

				indices[index++] = currentIndex;
				indices[index++] = nextIndex; 
			}
		}
		
		initialized = true;
	}

	void PlanetFace::draw()
	{
		if (!initialized)
			init();

		glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), vertices);
		glColorPointer(3, GL_FLOAT, sizeof(Vec3f), colors);
			
		glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, indices);
	
	
		/*
		
		Vec3f v1v2 = v2 - v1;
		Vec3f v1v4 = v4 - v1;

		const int DOTS = 4;

		for (int i = 0; i < DOTS; i++)
		{
			//glBegin(GL_TRIANGLE_STRIP);

			for (int j = 0; j < DOTS; j++)
			{
				float s = i/static_cast<float>(DOTS-1);
				float t = j/static_cast<float>(DOTS-1);
	
				float ss = (i+1)/static_cast<float>(DOTS-1);
				float tt = j/static_cast<float>(DOTS-1);
	
				Vec3f v = v1 + v1v2*s + v1v4*t;
				Vec3f vv = v1 + v1v2*ss + v1v4*tt;
				
				SpherePoint sp = v.toSpherePoint();
				sp.p = getHeight(s, t);
				Vec3f vSphere = sp.toVector();

				SpherePoint spp = vv.toSpherePoint();
				spp.p = getHeight(ss, tt);
				Vec3f vSpheree = spp.toVector();



				glColor3f(0.8f, 0.8f, 0.8f);
				//glVertex3f(v.x, v.y, v.z);

				glNormal3f(N.x, N.y, N.z);

				//glColor3f(0.0f, (sp.p-4.0f)/3.0f, 0.0f);
				//glVertex3f(vSphere.x, vSphere.y, vSphere.z);

				//glColor3f(0.0f, (spp.p-4.0f)/3.0f, 0.0f);
				//glVertex3f(vSpheree.x, vSpheree.y, vSpheree.z);

				//glVertex3f(v.x, v.y, v.z);
				//glVertex3f(vv.x, vv.y, vv.z);

				
				glBegin(GL_POINTS);
					glColor3f(0.8f, 0.8f, 0.8f);
					//glVertex3f(v.x, v.y, v.z);
					glVertex3f(vSphere.x, vSphere.y, vSphere.z);
				glEnd();
				
			
			}
			
			//glEnd();
		}
		*/
	}

	bool PlanetFace::findIntersection(SpherePoint &sp, float &s, float &t)
	{
		Vec3f v = sp.toVector();
		return findIntersection(v, s, t);
	}
	
	bool PlanetFace::findIntersection(Vec3f &p2, float &s, float &t)
	{
		Vec3f p1;
		return findIntersection(p1, p2, s, t); 
	}

	bool PlanetFace::findIntersection(Vec3f &p1, Vec3f &p2, float &s, float &t)
	{
		// plane equation: N * dot(p - p3) = 0
		// line equation: p = p1 + u * (p2 - p1)

		Vec3f p3 = v1;

		float u = N.dot(p3 - p1) / N.dot(p2 - p1);
		
		// Intersection point
		Vec3f p = u * (p2 - p1);

		// Check if intersection point is inside face
		Vec3f pv = p - v1;  // vector in plane
		Vec3f v1v2 = v2-v1;
		Vec3f v1v4 = v4-v1;
		
		s = pv.dot(v1v2) / v1v2.length();
		t = pv.dot(v1v4) / v1v4.length();
		
		bool insideFace = 
			(u >= 0) &&
			(0.0f <= s) && 
			(s <= v1v2.length()) && 
			(0.0f <= t) && 
			(t <= v1v4.length());
				
		
		s /= v1v2.length();
		t /= v1v4.length();

		/*
		// DEBUG
		printf("pv: (%f,%f,%f)\n", pv.x, pv.y, pv.z);
		printf("v1v2: (%f,%f,%f)\n", v1v2.x, v1v2.y, v1v2.z);
		printf("v1v4: (%f,%f,%f)\n", v1v4.x, v1v4.y, v1v4.z);
		//printf("u: %f, (%f,%f,%f)\n", u, p.x, p.y, p.z);
		printf("s: %f, t: %f\n\n", s, t);
		*/
		
		return insideFace;
	}
	
	float PlanetFace::getHeight(float s, float t)
	{
		if (s == 0.0f || s == 1.0f || t == 0.0f || t == 1.0f)
			return radius;
			
		return radius + sin(s*20.0f)/8.0f + cos(t*20.0f)/8.0f;
		//return radius + sin(s*5.0f + SDL_GetTicks()/400.0f)/2.0f;
	}
};

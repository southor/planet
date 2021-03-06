#include "PlanetFace.h"
#include "TextureHandler.h"

#include "GL/glext.h"

#define ENABLE_MULTITEXTURE_ARB

namespace Planet
{
	PlanetFace::PlanetFace(	float radius, 
							float detailScale, 
							float heightMapMulti,
							Pos v1, Pos v2, Pos v3, Pos v4, 
							const std::string &heightmapFile, 
							const std::string &textureFile, 
							const std::string &detailTextureFile) 
		:	radius(radius),
			detailScale(detailScale),
			heightMapMulti(heightMapMulti),
			resolution(40),
			initialized(false),
			v1(v1), 
			v2(v2), 
			v3(v3), 
			v4(v4), 
			sp1(v1), 
			sp2(v2), 
			sp3(v3), 
			sp4(v4),
			vertices(0),
			indices(0),
			heightmapFile(heightmapFile),
			textureFile(textureFile),
			detailTextureFile(detailTextureFile)
	{
	}


	void PlanetFace::init(bool isClient)
	{
		if (initialized)
			return;

		if (isClient)
		{
			texture = TextureHandler::loadTexture(textureFile);
			detailTexture = TextureHandler::loadTexture(detailTextureFile);
		}

		heightMap.init(heightmapFile);
		
		// create arrays
		numIndices = (resolution * resolution * 2) - (2 * resolution);
		
		vertices = new VertData[resolution * resolution];
		indices = new uint[numIndices];
	
		Vec v1v2 = v2 - v1;
		Vec v1v4 = v4 - v1;
		N = v1v2.cross(v1v4);

		// Fill up vertices and texture arrays
		for (int i = 0; i < resolution; i++)
		{
			for (int j = 0; j < resolution; j++)
			{
				int index = i * resolution + j;

				// s and t will be between 0.0 and 1.0
				float s = j/static_cast<float>(resolution-1);
				float t = i/static_cast<float>(resolution-1);
	
				Vec v = v1 + v1v2*s + v1v4*t;
				
				// Convert to spherepoint, set height and then back
				SpherePoint sp = v.toSpherePoint();
				sp.p = getHeight(s, t);
				Vec vSphere = sp.toVector();
	
				vertices[index].vertex = vSphere;
				vertices[index].textureCoords = Vec2f(s*1.0f, t*1.0f);
			}
		}
		
		// Fill up normal array
		for (int i = 0; i < resolution; i++)
		{
			for (int j = 0; j < resolution; j++)
			{
				int index = i * resolution + j;
				
				Vec p = getVertex(i, j);
				
				Vec pUp = getVertex(i - 1, j);
				Vec pLeft = getVertex(i, j - 1);
				Vec pDown = getVertex(i + 1, j);
				Vec pRight = getVertex(i, j + 1);
				
				Vec u1 = pRight - p;
				Vec u2 = pUp - p;
				Vec u3 = pLeft - p;
				Vec u4 = pDown - p;

				Vec n1 = u1.cross(u2);
				Vec n2 = u2.cross(u3);
				Vec n3 = u3.cross(u4);
				Vec n4 = u4.cross(u1);

				Vec normal = n1 + n2 + n3 + n4;
	
				normal.normalize();
				n4.normalize();
				
				vertices[i * resolution + j].normal = normal;
				vertices[i * resolution + j].planeNormal = n4;
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

	void PlanetFace::render()
	{
		bool useDetail = true;

		glEnable(GL_LIGHTING);


		#ifdef ENABLE_MULTITEXTURE_ARB
			#ifdef _WIN32
				PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = 0;
				glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)SDL_GL_GetProcAddress( "glActiveTextureARB" ) ;

				PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = 0;
				glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)SDL_GL_GetProcAddress( "glClientActiveTextureARB" ) ;
			#endif

			glVertexPointer(3, GL_FLOAT, sizeof(VertData), vertices);
			glNormalPointer(GL_FLOAT, sizeof(VertData), &(vertices[0].normal));
			
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(VertData), &(vertices[0].textureCoords));
			glBindTexture(GL_TEXTURE_2D, detailTexture);
			
			glEnable(GL_TEXTURE_2D); 
			glBindTexture(GL_TEXTURE_2D, texture);

			if (useDetail)
			{
				glActiveTextureARB(GL_TEXTURE1_ARB);
				glEnable(GL_TEXTURE_2D);
				glClientActiveTextureARB(GL_TEXTURE1_ARB);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(VertData), &(vertices[0].textureCoords));

				glMatrixMode(GL_TEXTURE);

				glLoadIdentity();
				glScalef(detailScale, detailScale, 1.0f);

				glMatrixMode(GL_MODELVIEW);

				glBindTexture(GL_TEXTURE_2D, detailTexture);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
			}
			
			glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, indices);
			
			// Disable
			if (useDetail)
			{
				glActiveTextureARB(GL_TEXTURE1_ARB);
				glDisable(GL_TEXTURE_2D);

				glActiveTextureARB(GL_TEXTURE0_ARB);
				glClientActiveTextureARB(GL_TEXTURE0_ARB);
			}
		#endif

		#ifndef ENABLE_MULTITEXTURE_ARB
			// OLD - without multitexturing/detail
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glVertexPointer(3, GL_FLOAT, sizeof(VertData), vertices);
			glNormalPointer(GL_FLOAT, sizeof(VertData), &(vertices[0].normal));
			glTexCoordPointer(2, GL_FLOAT, sizeof(VertData), &(vertices[0].textureCoords));

			glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, indices);
		#endif
	}

	bool PlanetFace::findIntersection(SpherePoint &sp, float &s, float &t)
	{
		Vec3f v = sp.toVector();
		return findIntersection(v, s, t);
	}
	
	bool PlanetFace::findIntersection(Vec3f &p2, float &s, float &t)
	{
		Vec3f p1(0.0f, 0.0f, 0.0f);
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
		Vec3f v1v2 = v2 - v1;
		Vec3f v1v4 = v4 - v1;
		
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

		return insideFace;
	}
	
	float PlanetFace::getHeight(float s, float t)
	{
		if (s == 1.0f) s -= 0.0001;
		if (t == 1.0f) t -= 0.0001;

		/*
		//
		// Interpolation code
		//
		
		float height;
		float res = static_cast<float>(resolution - 1);

		// Get s,t representing top/left vertex (0.44 with resolution 10 becomes 0.4)
		float s0 = static_cast<float>(static_cast<int>(s * res) / res);
		float t0 = static_cast<float>(static_cast<int>(t * res) / res);

		// Get s,t representing bottom,right vertex (s0 = 0.4 and resolution 10 gives s1 = 0.5)
		float s1 = s0 + 1 / res;
		float t1 = t0 + 1 / res;
		
		assert(s0 >= 0.0f && s0 <= 1.0f);
		assert(t0 >= 0.0f && t0 <= 1.0f);
		assert(s1 >= 0.0f && s1 <= 1.0f);
		assert(t1 >= 0.0f && t1 <= 1.0f);
		
		
		// Heights of the four nearby vertices 
		float height0s0t = heightMap.getHeight(s0, t0);
		float height1s0t = heightMap.getHeight(s1, t0);
		float height0s1t = heightMap.getHeight(s0, t1);
		float height1s1t = heightMap.getHeight(s1, t1);


		// Value between 0.0 and 1.0 which tells us where s is between s0 and s1
		// s0 = 0.4, s1 = 0.5 and s = 0.47 becomes 0.7
		float sBetween = (s - s0) * res;
		float tBetween = (t - t0) * res;
		
		
		assert(sBetween >= 0.0f && sBetween <= 1.0f);
		assert(tBetween >= 0.0f && tBetween <= 1.0f);
		
		// Check which vertices we should interpolate between
		if (sBetween + tBetween <= 1.0f)
		{
			height = height0s0t + (height1s0t - height0s0t) * sBetween + (height0s1t - height0s0t) * tBetween;
		}
		else
		{
			height = height1s1t + (height0s1t - height1s1t) * (1 - sBetween) + (height1s0t - height1s1t) * (1 - tBetween);
		}
		*/
		

		return radius + heightMap.getHeight(s, t) * 3.0f * heightMapMulti;
	

		//return radius + sin(s*20.0f)/8.0f + cos(t*20.0f)/8.0f;
		//return radius + sin(s*5.0f + SDL_GetTicks()/400.0f)/2.0f;
	}
	
	VertData PlanetFace::getVertexData(float s, float t)
	{
		int row = static_cast<int>(t * (resolution - 1));
		int col = static_cast<int>(s * (resolution - 1));

		return vertices[row * resolution + col];
	}
	
	Vec3f PlanetFace::getVertex(int row, int col)
	{
		if (row < 0) row = 0;
		if (row >= resolution) row = resolution - 1;

		if (col < 0) col = 0;
		if (col >= resolution) col = resolution - 1;
		
		return vertices[row * resolution + col].vertex;
	}

};

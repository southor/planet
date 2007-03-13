#ifndef __PlanetFace_h__
#define __PlanetFace_h__

#include "common.h"
#include "basic.h"
#include "HeightMap.h"

namespace Planet
{
	class PlanetFace
	{
	public:
		PlanetFace(float r, Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4, const std::string &heightmapFile, const std::string &textureFile);

		~PlanetFace()
		{
			delete [] vertices;
			delete [] colors;
			delete [] normals;
			delete [] indices;
		}

		void init();

		void draw();

		float getHeight(float s, float t);

		// Checks intersection between the line sp-(0,0,0) and this planet face/side
		bool findIntersection(SpherePoint &sp, float &s, float &t);

		// Checks intersection between the line p2-(0,0,0) and this planet face/side
		bool findIntersection(Vec3f &p2, float &s, float &t);

		// Checks intersection between the line p2-p1 and this planet face/side
		bool findIntersection(Vec3f &p1, Vec3f &p2, float &s, float &t);


		Vec3f getVertex(int row, int col);

	private:
		// Vertexes (corners) in clockwise order
		Vec3f v1;
		Vec3f v2;
		Vec3f v3;
		Vec3f v4;

		// Normal of this Face/Plane
		Vec3f N;

		SpherePoint sp1;
		SpherePoint sp2;
		SpherePoint sp3;
		SpherePoint sp4;
		
		float radius;

		int resolution;
		
		int numIndices;
		
		bool initialized;
		
		// vertex array
		Vec3f *vertices;
		
		// color array
		Vec3f *colors;

		// index array
		uint *indices;
		
		// normal array
		Vec3f *normals;
		
		Vec2f *textureCoords;
		
		HeightMap heightMap;
		
		uint texture;
		
		std::string heightmapFile;
		std::string textureFile;
	};
};


#endif
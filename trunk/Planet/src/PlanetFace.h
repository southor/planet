#ifndef __PlanetFace_h__
#define __PlanetFace_h__

#include "common.h"
#include "basic.h"
#include "HeightMap.h"

namespace Planet
{
	struct VertData
	{
		Vec vertex;
		Vec normal;
		Vec planeNormal;
		Vec2f textureCoords;
	};

	class PlanetFace
	{
	public:
		PlanetFace() : vertices(0), indices(0) {}
		PlanetFace(	float radius, 
					float detailScale,
					float heightMapMulti,
					Pos v1, Pos v2, Pos v3, Pos v4, 
					const std::string &heightmapFile, 
					const std::string &textureFile, 
					const std::string &detailTextureFile);

		~PlanetFace()
		{
			delete [] vertices;
			delete [] indices;
		}

		void init();

		void render();

		// Checks intersection between the line sp-(0,0,0) and this planet face/side
		bool findIntersection(SpherePoint &sp, float &s, float &t);

		// Checks intersection between the line p2-(0,0,0) and this planet face/side
		bool findIntersection(Vec &p2, float &s, float &t);

		// Checks intersection between the line p2-p1 and this planet face/side
		bool findIntersection(Vec &p1, Vec3f &p2, float &s, float &t);

		float getHeight(float s, float t);

		VertData getVertexData(float s, float t);

		Vec getVertex(int row, int col);

	private:
		// Vertexes (corners) in clockwise order
		Pos v1;
		Pos v2;
		Pos v3;
		Pos v4;

		// Normal of this Face/Plane
		Vec N;

		SpherePoint sp1;
		SpherePoint sp2;
		SpherePoint sp3;
		SpherePoint sp4;
		
		float radius;
		float detailScale;
		float heightMapMulti;

		int resolution;
		
		int numIndices;
		
		bool initialized;
		
		VertData *vertices;

		// index array
		uint *indices;
		
		HeightMap heightMap;
		
		uint texture;
		uint detailTexture;
		
		std::string heightmapFile;
		std::string textureFile;
		std::string detailTextureFile;
	};
};


#endif
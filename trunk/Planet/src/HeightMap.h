#ifndef __HeightMap_h__
#define __HeightMap_h__

#include "common.h"
#include "basic.h"

namespace Planet
{
	class HeightMap
	{
	public:
		HeightMap() : map(0) {}
		~HeightMap();
		
		void init(char *image);
		
		void draw(GLuint texture);

		float getHeight(float s, float t);

	private:
		SDL_Surface *map;

		float *floorHeights;

		int width;
		int depth;
	};
};

#endif

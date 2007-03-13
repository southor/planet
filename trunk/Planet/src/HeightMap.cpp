#include "HeightMap.h"

#define WORLD_SIZE 30

namespace Planet
{
	void HeightMap::init(const std::string &image)
	{
		map = IMG_Load(("images/" + image).c_str());
		if (!map)
		{
			printf("IMG_Load: %s\n", IMG_GetError());
		}

		width = map->w;
		depth = map->h;

		floorHeights = new float[width * depth];

		unsigned char* pixels = (unsigned char*)map->pixels;
		int pitch = map->pitch;
		int mapbpp = map->format->BytesPerPixel;

		for (int z = 0; z < depth; z++)
		{
			for (int x = 0; x < width; x++)
			{
				double floorDepth = (pixels[x * mapbpp + z * pitch + 0] / 255.0f);

				floorHeights[x + z * width] = (float)(floorDepth);
			}
		}
	}
	
	float HeightMap::getHeight(float s, float t)
	{
		int z = s * depth;
		int x = t * width;
		
		return floorHeights[x + z * width];
	}

	HeightMap::~HeightMap()
	{
		SDL_FreeSurface(map);

		delete [] floorHeights;
	}
};

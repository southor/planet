#ifndef __TextureHandler_h__
#define __TextureHandler_h__

#include "common.h"
#include "SDL_OpenGL.h"
#include <string>

namespace Planet
{
	class TextureHandler
	{
	public:
		static GLuint loadTexture(const std::string &file)
		{
			SDL_Surface* tmp = IMG_Load(file.c_str());

			if (tmp == NULL)
			{
					printf("Unable to load: %s\n", file.c_str());
					exit(-1);
			}
			
			SDL_Surface* colordata = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32,
															SDL_SwapLE32(0x000000ff),
															SDL_SwapLE32(0x0000ff00),
															SDL_SwapLE32(0x00ff0000),
															SDL_SwapLE32(0xff000000));

			SDL_Rect rect;
			rect.x = 0;
			rect.y = 0;
			rect.w = tmp->w;
			rect.h = tmp->h;
		  
			SDL_Surface* image = SDL_ConvertSurface(tmp, colordata->format, SDL_SWSURFACE);
			SDL_FreeSurface(colordata);

			SDL_FreeSurface(tmp);

			GLuint textureHandle;
			glGenTextures(1, &textureHandle);
			glBindTexture(GL_TEXTURE_2D, textureHandle);


			gluBuild2DMipmaps(GL_TEXTURE_2D,
						GL_RGBA,      
						image->w,     
						image->h,     
						GL_RGBA,      
						GL_UNSIGNED_BYTE,
						image->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			return textureHandle;
		}
	};
};

#endif

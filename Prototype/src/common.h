#ifndef __common_h__
#define __common_h__

#ifdef _WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_net.lib")
//#pragma comment(lib, "SDL_mixer.lib")
#pragma comment( lib, "opengl32.lib" )
//#pragma comment( lib, "glu32.lib" )
//#pragma comment( lib, "glaux.lib" )

#endif

#if defined _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef DELETE
#endif

//#include <string>
//#include <cstdlib>
#include <iostream>


#ifdef _WIN32
#include <GL/gl.h>
#endif
#ifdef __APPLE__
#include "OpenGL/gl.h"
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
//#include "SDL_mixer.h"
//#include <GL/glu.h>
//#include "vec3f.h"
//#include <math.h>

//#define dmin(x, y) (x < y ? x : y)
//#define dmax(x, y) (x > y ? x : y)

#include "TimeHandler.h"

namespace Prototype
{
	//static TimeHandler timeHandler;

	//debug
	//static std::ostream debugOutput;
};




#endif

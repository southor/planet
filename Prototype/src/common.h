#ifndef __common_h__
#define __common_h__

#ifdef _WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
//#pragma comment(lib, "SDL_image.lib")
//#pragma comment(lib, "SDL_mixer.lib")
#pragma comment( lib, "opengl32.lib" )
//#pragma comment( lib, "glu32.lib" )
//#pragma comment( lib, "glaux.lib" )
#endif

#if defined _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//#include <string>
//#include <cstdlib>

#ifdef _WIN32
#include <GL/gl.h>
#endif
//#include <GL/glu.h>
#ifndef _WIN32
#include "SDL_OpenGL.h"
#endif

#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "vec3f.h"
//#include <math.h>


//#define WORLD_SIZE 200
//#define OPTIMIZE_FROSTUM true


//#define dmin(x, y) (x < y ? x : y)
//#define dmax(x, y) (x > y ? x : y)

#endif

#ifndef __common_h__
#define __common_h__

#ifdef _WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_net.lib")
//#pragma comment(lib, "SDL_mixer.lib")
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
//#pragma comment( lib, "glaux.lib" )

#endif

#if defined _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef DELETE
#endif

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

//#define GL_GLEXT_PROTOTYPES 1
//#include "GL/glext.h"

typedef void (*GL_ActiveTextureARB_Func)(unsigned int);
GL_ActiveTextureARB_Func glActiveTextureARB = (GL_ActiveTextureARB_Func)SDL_GL_GetProcAddress("glActiveTextureARB");

typedef void (*GL_ClientActiveTextureARB_Func)(unsigned int);
GL_ClientActiveTextureARB_Func glClientActiveTextureARB = (GL_ClientActiveTextureARB_Func)SDL_GL_GetProcAddress("glClientActiveTextureARB");

#endif


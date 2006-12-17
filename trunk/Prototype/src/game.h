#ifndef __game_h__
#define __game_h__

//#include <vector>

#include "common.h"
#include "keyhandler.h"
#include "virtualconnection.h"
#include "client.h"
#include "server.h"

namespace Prototype
{

	class Game 
	{
	public:
		Game();
		~Game();
		void run();

	private:
		void init();

		void draw(Uint32 time);
		void render(Uint32 time);
		void pollEvents();
		Uint32 getTicks();

		bool running;
		SDL_Surface *screen;
		GLfloat renv;

		KeyHandler kh;
		VirtualConnection virtualConnection1;
		VirtualConnection virtualConnection2;
		Client client;

		float xrel;
		float yrel;	
		GLfloat cameraXAngle;
		GLfloat cameraYAngle;

		Uint32 mStartTime;
	};
};

#endif

#ifndef __game_h__
#define __game_h__

//#include <vector>

#include "common.h"
#include "keyhandler.h"

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

		float xrel;
		float yrel;	
		GLfloat cameraXAngle;
		GLfloat cameraYAngle;

		Uint32 mStartTime;
	};
};

#endif

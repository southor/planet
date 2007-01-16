#ifndef __game_h__
#define __game_h__

//#include <vector>

#include "common.h"
#include "keyhandler.h"
#include "virtualconnection.h"
#include "client.h"
#include "server.h"
//#include "Gui.h"

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
		void initWidgets();

		void draw(Uint32 time);
		void render(Uint32 time);
		void pollEvents();

		bool running;
		SDL_Surface *screen;
		GLfloat renv;

		VirtualConnection virtualConnection1;
		VirtualConnection virtualConnection2;
		VirtualConnection virtualConnection3;
		VirtualConnection virtualConnection4;
		Client client1;
		Client client2;

//		Gui gui;
	};
};

#endif

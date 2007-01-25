#ifndef __game_h__
#define __game_h__

//#include <vector>

#include "common.h"
#include "keyhandler.h"
#include "virtualconnection.h"
#include "client.h"
#include "server.h"
#include "Gui.h"

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
		Uint32 getTicks();

		bool running;
		SDL_Surface *screen;
		GLfloat renv;

		VirtualConnection virtualConnection1;
		VirtualConnection virtualConnection2;
		VirtualConnection virtualConnection3;
		VirtualConnection virtualConnection4;

		Server server;

		Client client1;
		Client client2;

		//// used for storing viewport and translating positions from screen to game
		//ViewportHandler viewportHandler1;
		//ViewportHandler viewportHandler1;

		Gui gui;

		Uint32 mStartTime;

		static const Vec2<int> WINDOW_SIZE;
	};
};

#endif

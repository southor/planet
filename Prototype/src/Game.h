#ifndef __game_h__
#define __game_h__

//#include <vector>

#include "common.h"
#include "KeyHandler.h"
#include "VirtualConnection.h"
#include "Client.h"
#include "Server.h"
//#include "Gui.h"
#include "UserInputHandler.h"
#include "Planet.h"

namespace Prototype
{
	class Game 
	{
	public:
		Game();
		~Game();
		void run(std::string &host);
		void run2();

		static const Vec2<int> WINDOW_SIZE;

		static const int SERVER_TO_CLIENT_1_SIMULATED_LAG = 120;
		static const int SERVER_TO_CLIENT_2_SIMULATED_LAG = 110;
		static const int CLIENT_1_TO_SERVER_SIMULATED_LAG = 120;
		static const int CLIENT_2_TO_SERVER_SIMULATED_LAG = 120;
		

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

		//Gui gui;

		Uint32 mStartTime;

		Planet planet;

		
	};
};

#endif


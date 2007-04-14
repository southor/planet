#ifndef __game_h__
#define __game_h__

#include "common.h"
#include "KeyHandler.h"
#include "VirtualConnection.h"
#include "Client.h"
#include "Server.h"
#include "UserInputHandler.h"

namespace Prototype
{
	class Game 
	{
	public:
		Game();
		~Game();
		void run(std::string &host);

		static const Vec2<int> WINDOW_SIZE;

		// init simulated lag
		static const int SERVER_TO_CLIENT_1_INIT_SIMULATED_LAG_DEFAULT = 50;
		static const int CLIENT_1_TO_SERVER_INIT_SIMULATED_LAG_DEFAULT = 50;
		// init simulated lag
		static const int SERVER_TO_CLIENT_2_INIT_SIMULATED_LAG_DEFAULT = 200;
		static const int CLIENT_2_TO_SERVER_INIT_SIMULATED_LAG_DEFAULT = 200;

		// final simulated lag
		static const int SERVER_TO_CLIENT_1_FINAL_SIMULATED_LAG_DEFAULT = 50;
		static const int CLIENT_1_TO_SERVER_FINAL_SIMULATED_LAG_DEFAULT = 50;

		// final simulated lag
		static const int SERVER_TO_CLIENT_2_FINAL_SIMULATED_LAG_DEFAULT = 200;
		static const int CLIENT_2_TO_SERVER_FINAL_SIMULATED_LAG_DEFAULT = 200;
		
	private:

		static const int RENDER_AT_LEAST_EVERY_TICK_DEFAULT = 0;

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

		Uint32 mStartTime;

		ConfigHandler configHandler;
	};
};

#endif


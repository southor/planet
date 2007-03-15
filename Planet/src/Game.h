#ifndef __Game_h__
#define __Game_h__

#include "common.h"
#include "basic.h"
#include "UserInputHandler.h"
#include "Client.h"
#include "NetworkConnection.h"
#include "Gui.h"

#include "SDL_thread.h"

namespace Planet
{
	class Game 
	{
	public:
		Game();
		~Game();
		void run();

		static const Vec2<int> WINDOW_SIZE;

		void startServer(int clients);
		void connectToServer(std::string &host);

		void toggleMenu() {	showMenu = showMenu ^ 1; }

	private:
		void init();

		void render(Uint32 time);
		void pollEvents();
	
		bool running;
		bool connectedToServer;

		bool showMenu;

		SDL_Surface *screen;

		Client client;

		SDL_Thread *serverThread;

		NetworkClient networkClient;

		Gui gui;


		// TMP / DEBUG
		float viewAngle;
		float viewAngle2;
	};
};

#endif


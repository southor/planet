#ifndef __Game_h__
#define __Game_h__

#include "common.h"
#include "basic.h"
#include "UserInputHandler.h"
#include "Client.h"

namespace Planet
{
	class Game 
	{
	public:
		Game();
		~Game();
		void run();

		static const Vec2<int> WINDOW_SIZE;

	private:
		void init();

		void render(Uint32 time);
		void pollEvents();

		bool running;
		SDL_Surface *screen;

		Client client;

		SDL_Thread *serverThread;

		// TMP / DEBUG
		float viewAngle;
		float viewAngle2;
	};
};

#endif


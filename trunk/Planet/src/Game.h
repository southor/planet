#ifndef __Game_h__
#define __Game_h__

#include "common.h"
#include "basic.h"
#include "UserInputHandler.h"
#include "PlanetBody.h"
#include "Ship.h"
#include "Camera.h"
#include "Sight.h"

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600

namespace Planet
{
	class Game 
	{
	public:
		Game();
		~Game();
		void run();

	private:
		void init();

		void render(Uint32 time);
		void pollEvents();

		bool running;
		SDL_Surface *screen;

		PlanetBody planetBody;
		Ship ship;
		Camera camera;
		Sight sight;
		
		// TMP / DEBUG
		float viewAngle;
		float viewAngle2;

		
		UserInputHandler userInputHandler;
	};
};

#endif


#include "Game.h"
#include "Cmds.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

namespace Planet
{
	Game::Game()
	{
		init();
		running = true;
	}

	Game::~Game()
	{
		SDL_Quit( );
		SDLNet_Quit();
	}

	void Game::run()
	{
		while (running) 
		{
			pollEvents();

			glClear(GL_COLOR_BUFFER_BIT);
			
			if (userInputHandler.getCurrentState(Cmds::LEFT))
				planet.viewAngle += 3.0f;
			if (userInputHandler.getCurrentState(Cmds::RIGHT))
				planet.viewAngle -= 3.0f;

			if (userInputHandler.getCurrentState(Cmds::FORWARD))
				planet.viewAngle2 += 3.0f;
			if (userInputHandler.getCurrentState(Cmds::BACKWARD))
				planet.viewAngle2 -= 3.0f;

			if (userInputHandler.getCurrentState(Cmds::TMP_LEFT))
				planet.shipPhi += 0.05f;
			if (userInputHandler.getCurrentState(Cmds::TMP_RIGHT))
				planet.shipPhi -= 0.05f;

			if (userInputHandler.getCurrentState(Cmds::TMP_UP))
				planet.shipTheta += 0.05f;
			if (userInputHandler.getCurrentState(Cmds::TMP_DOWN))
				planet.shipTheta -= 0.05f;
		

			render(0);

			SDL_Delay(10);
		}
	}

	void Game::render(Uint32 time)
	{
		glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y); 

		glDisable(GL_LIGHTING);

		planet.render();
		
		glEnable(GL_LIGHTING);
		glFlush();
		SDL_GL_SwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Game::init()
	{
		running = true;

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
			SDL_Quit();
		}

		if (SDLNet_Init() < 0)
		{
			fprintf(stderr, "Network initialization failed: %s\n", SDL_GetError());
			SDLNet_Quit();		
		}
  
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

		
		uint flags = SDL_HWSURFACE | SDL_OPENGL; // |SDL_FULLSCREEN;
		uint w = WINDOW_SIZE_X;
		uint h = WINDOW_SIZE_Y;

		uint bpp = 32;	

		screen = SDL_SetVideoMode(w, h, bpp, flags);
		if (screen == 0)
		{
			//throw EXCEPTION("Failed to set video mode");
		}

		SDL_ShowCursor(SDL_ENABLE);
		glViewport(0, 0, w, h);
		
		SDL_EnableUNICODE(1);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);	


		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);



		glShadeModel(GL_SMOOTH);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1.0f);
		glClearAccum(0.0, 0.0, 0.0, 0.0);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


		userInputHandler.setStateCmdKey(Cmds::LEFT, SDLK_a);
		userInputHandler.setStateCmdKey(Cmds::RIGHT, SDLK_d);
		userInputHandler.setStateCmdKey(Cmds::FORWARD, SDLK_w);
		userInputHandler.setStateCmdKey(Cmds::BACKWARD, SDLK_s);
		userInputHandler.setStateCmdKey(Cmds::ROTATE_LEFT, SDLK_c);
		userInputHandler.setStateCmdKey(Cmds::ROTATE_RIGHT, SDLK_v);
		userInputHandler.setActionCmdKey(Cmds::SHOOT, SDLK_SPACE);
		userInputHandler.setActionCmdKey(Cmds::SWITCH_WEAPON, SDLK_x);

		userInputHandler.setStateCmdKey(Cmds::TMP_LEFT, SDLK_LEFT);
		userInputHandler.setStateCmdKey(Cmds::TMP_RIGHT, SDLK_RIGHT);
		userInputHandler.setStateCmdKey(Cmds::TMP_UP, SDLK_UP);
		userInputHandler.setStateCmdKey(Cmds::TMP_DOWN, SDLK_DOWN);


		userInputHandler.aimMode = UserInputHandler::KEYBOARD;
	}

	void Game::pollEvents()
	{
		SDL_Event event;
		
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
			{
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;				   
				case SDLK_f:					
					if (event.key.keysym.mod & KMOD_CTRL)
						SDL_WM_ToggleFullScreen(screen);
					break;
				}
				break;
			}
		
			case SDL_QUIT:
				running = false;
				break;
			}

			userInputHandler.pushInput(event);
		}
		
	}
};

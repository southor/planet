#include "Game.h"
#include "Cmds.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

namespace Planet
{
	Game::Game() : viewAngle(0.0f), viewAngle2(0.0f)
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
				viewAngle += 3.0f;
			if (userInputHandler.getCurrentState(Cmds::RIGHT))
				viewAngle -= 3.0f;

			if (userInputHandler.getCurrentState(Cmds::FORWARD))
				viewAngle2 += 3.0f;
			if (userInputHandler.getCurrentState(Cmds::BACKWARD))
				viewAngle2 -= 3.0f;

			ship.moveUp = userInputHandler.getCurrentState(Cmds::TMP_UP);
			ship.moveDown = userInputHandler.getCurrentState(Cmds::TMP_DOWN);
			ship.moveLeft = userInputHandler.getCurrentState(Cmds::TMP_LEFT);
			ship.moveRight = userInputHandler.getCurrentState(Cmds::TMP_RIGHT);
				
			if (userInputHandler.getCurrentState(Cmds::TMP_ZOOM_IN))
				camera.zoom += 0.05f;
			if (userInputHandler.getCurrentState(Cmds::TMP_ZOOM_OUT))
				camera.zoom -= 0.05f;
		
			Vec2<int> mouseScreenPos = userInputHandler.getMouseScreenPos();
		
			planet.mouseScreenPosRel = Vec2<float>(
				mouseScreenPos.x / static_cast<float>(WINDOW_SIZE_X), 
				mouseScreenPos.y / static_cast<float>(WINDOW_SIZE_Y));


			ship.logic();



			camera.update(ship.position, ship.reference);
			sight.update(planet.mouseScreenPosRel);


			ship.direction = sight.position - ship.position;

			
			render(0);

			SDL_Delay(10);
		}
	}

	void Game::render(Uint32 time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y); 

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);

		camera.useCamera();

		glPushMatrix();
			glRotatef(viewAngle, 0.0f, 1.0f, 0.0f);
			glRotatef(viewAngle2, 1.0f, 0.0f, 0.0f);

			float light_ambient[] = {0.3, 0.3, 0.3, 1.0};
			float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
			float light_specular[] = {1.0, 0.0, 0.0, 1.0};
			float light_position[] = {00.0, 10.0, 0.0, 0.0};
			
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);

			glEnable(GL_LIGHT0);



			planet.render();

			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);

			ship.render();
			sight.render();

		glPopMatrix();
			
		glFlush();
		SDL_GL_SwapBuffers();
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
		glEnableClientState(GL_NORMAL_ARRAY);	
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glShadeModel(GL_SMOOTH);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClearDepth(1.0f);
		//glClearAccum(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

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
		userInputHandler.setStateCmdKey(Cmds::TMP_ZOOM_IN, SDLK_q);
		userInputHandler.setStateCmdKey(Cmds::TMP_ZOOM_OUT, SDLK_e);


		userInputHandler.aimMode = UserInputHandler::KEYBOARD;
		
		
		planet.init();
		ship.setPlanet(&planet);
		sight.setCamera(&camera);
		sight.setPlanet(&planet);

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

#include "Game.h"
#include "Cmds.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"
#include "SDL_thread.h"

namespace Planet
{
	bool runningServer;

	int runServer(void *unused)
	{
		while (runningServer)
		{
			printf("runServer(..)\n");
			SDL_Delay(1000);
		}

		return 0;
	}


	const Vec2<int> Game::WINDOW_SIZE = Vec2<int>(800, 600);
	
	Game::Game() : viewAngle(0.0f), viewAngle2(0.0f)
	{
		init();
		running = true;
	}

	Game::~Game()
	{
		SDL_WaitThread(serverThread, NULL);

		SDL_Quit( );
		SDLNet_Quit();
	}

	void Game::run()
	{
		while (running) 
		{
			pollEvents();
			
			if (client.getUserInputHandler()->getCurrentState(Cmds::LEFT))
				viewAngle += 3.0f;
			if (client.getUserInputHandler()->getCurrentState(Cmds::RIGHT))
				viewAngle -= 3.0f;

			if (client.getUserInputHandler()->getCurrentState(Cmds::FORWARD))
				viewAngle2 += 3.0f;
			if (client.getUserInputHandler()->getCurrentState(Cmds::BACKWARD))
				viewAngle2 -= 3.0f;

			//if (client.getUserInputHandler()->getCurrentState(Cmds::TMP_ZOOM_IN))
			//	camera.zoom += 0.05f;
			//if (client.getUserInputHandler()->getCurrentState(Cmds::TMP_ZOOM_OUT))
			//	camera.zoom -= 0.05f;


			
			render(0);

			SDL_Delay(10);
		}
	}

	void Game::render(Uint32 time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y); 
		glViewport(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y); 

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);

		client.getCamera()->useCamera();

		glPushMatrix();
			glRotatef(viewAngle, 0.0f, 1.0f, 0.0f);
			glRotatef(viewAngle2, 1.0f, 0.0f, 0.0f);

			// Setup lights
			float light_ambient[] = {0.3, 0.3, 0.3, 1.0};
			float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
			float light_specular[] = {1.0, 0.0, 0.0, 1.0};
			float light_position[] = {00.0, 10.0, 0.0, 0.0};
			
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);

			glEnable(GL_LIGHT0);

			client.renderAndUpdate();			


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
		//uint w = WINDOW_SIZE_X;
		//uint h = WINDOW_SIZE_Y;
		uint w = WINDOW_SIZE.x;
		uint h = WINDOW_SIZE.y;

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


		client.getUserInputHandler()->setStateCmdKey(Cmds::LEFT, SDLK_a);
		client.getUserInputHandler()->setStateCmdKey(Cmds::RIGHT, SDLK_d);
		client.getUserInputHandler()->setStateCmdKey(Cmds::FORWARD, SDLK_w);
		client.getUserInputHandler()->setStateCmdKey(Cmds::BACKWARD, SDLK_s);
		client.getUserInputHandler()->setStateCmdKey(Cmds::ROTATE_LEFT, SDLK_c);
		client.getUserInputHandler()->setStateCmdKey(Cmds::ROTATE_RIGHT, SDLK_v);
		//client.getUserInputHandler()->.setActionCmdKey(Cmds::SHOOT, SDLK_SPACE);
		client.getUserInputHandler()->setActionCmdKey(Cmds::START_SHOOTING, Cmds::STOP_SHOOTING, SDL_BUTTON_LEFT);
		client.getUserInputHandler()->setActionCmdKey(Cmds::SWITCH_WEAPON, SDLK_x);

		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_LEFT, SDLK_LEFT);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_RIGHT, SDLK_RIGHT);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_UP, SDLK_UP);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_DOWN, SDLK_DOWN);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_ZOOM_IN, SDLK_q);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_ZOOM_OUT, SDLK_e);


		client.getUserInputHandler()->aimMode = UserInputHandler::KEYBOARD;
		
		client.init();


		runningServer = true;	
		serverThread = SDL_CreateThread(runServer, 0);

		if (serverThread == 0)
		{
			fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
	        exit(-1);
		}
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
					runningServer = false;
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

			client.getUserInputHandler()->pushInput(event);
		}		
	}
};

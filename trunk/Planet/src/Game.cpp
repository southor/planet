#include "Game.h"
#include "Cmds.h"
#include "Server.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

namespace Planet
{
	bool runningServer = false;
	int numberOfClients = 0;

	int runServer(void *unused)
	{
		printf("Starting server...\n");

		Server server;
		server.run(runningServer, numberOfClients);

		return 0;
	}


	const Vec2<int> Game::WINDOW_SIZE = Vec2<int>(800, 600);
	
	Game::Game() : viewAngle(0.0f), viewAngle2(0.0f), serverThread(0), connectedToServer(false), running(true), showMenu(false)
	{
		init();
	}

	Game::~Game()
	{
		if (serverThread)
			SDL_WaitThread(serverThread, NULL);

		SDL_Quit( );
		SDLNet_Quit();
	}

	void Game::run()
	{
		std::string host("localhost");

		while (running) 
		{
			pollEvents();
			gui.logic();
			client.runStep();

			render(0);


			if (client.getUserInputHandler()->getCurrentState(Cmds::START_SERVER))
				startServer(1);

			if (client.getUserInputHandler()->getCurrentState(Cmds::CONNECT_TO_SERVER))
				connectToServer(host);

			if (client.getUserInputHandler()->hasActionCmdOnQueue())
			{
				//pushActionCmd(int actionCmd)
				int actionCmd = client.getUserInputHandler()->popActionCmd();
				if (actionCmd == Cmds::TOGGLE_MENU) toggleMenu();
				else client.getUserInputHandler()->pushActionCmd(actionCmd);
			}
			//// Workaround using both state and action cmd
			//if (client.getUserInputHandler()->getCurrentState(Cmds::TOGGLE_MENU))
			//	if (client.getUserInputHandler()->hasActionCmdOnQueue())
			//		if (client.getUserInputHandler()->popActionCmd() == Cmds::TOGGLE_MENU)
			//			toggleMenu();

			
			if (client.getUserInputHandler()->getCurrentState(Cmds::LEFT))
				viewAngle += 3.0f;
			if (client.getUserInputHandler()->getCurrentState(Cmds::RIGHT))
				viewAngle -= 3.0f;

			if (client.getUserInputHandler()->getCurrentState(Cmds::FORWARD))
				viewAngle2 += 3.0f;
			if (client.getUserInputHandler()->getCurrentState(Cmds::BACKWARD))
				viewAngle2 -= 3.0f;

			if (client.getUserInputHandler()->getCurrentState(Cmds::TMP_ZOOM_IN))
				client.camera.zoom += 0.05f;
			if (client.getUserInputHandler()->getCurrentState(Cmds::TMP_ZOOM_OUT))
				client.camera.zoom -= 0.05f;

			SDL_Delay(10);
		}
	}

	void Game::render(Uint32 time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y); 
		glViewport(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y); 

		client.getCamera()->useCamera();


		skyBox.render();


		glPushMatrix();

			glRotatef(viewAngle, 0.0f, 1.0f, 0.0f);
			glRotatef(viewAngle2, 1.0f, 0.0f, 0.0f);

			float redAmbient = 0.2f;

			if (!connectedToServer)
				redAmbient = 0.8f;


			// Setup lights
			float light_ambient[] = {redAmbient, 0.2f, 0.2f, 1.0f};
			float light_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
			float light_specular[] = {1.0f, 0.0f, 0.0f, 1.0f};
			float light_position[] = {0.0f, 10.0f, 0.0f, 0.0f};
			
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);

			glEnable(GL_LIGHT0);
				
			client.renderAndUpdate();			




		glPopMatrix();
			
		if (showMenu)
			gui.render();

			
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
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
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
		//client.getUserInputHandler()->setActionCmdKey(Cmds::SHOOT, SDLK_SPACE);
		client.getUserInputHandler()->setActionCmdKey(Cmds::START_SHOOTING, Cmds::STOP_SHOOTING, SDL_BUTTON_LEFT);
		client.getUserInputHandler()->setActionCmdKey(Cmds::SWITCH_WEAPON, SDLK_x);

		client.getUserInputHandler()->setStateCmdKey(Cmds::TOGGLE_MENU, SDLK_F1);
		client.getUserInputHandler()->setActionCmdKey(Cmds::TOGGLE_MENU, SDLK_F1);

		client.getUserInputHandler()->setStateCmdKey(Cmds::START_SERVER, SDLK_F2);
		client.getUserInputHandler()->setStateCmdKey(Cmds::CONNECT_TO_SERVER, SDLK_F3);

		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_LEFT, SDLK_LEFT);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_RIGHT, SDLK_RIGHT);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_UP, SDLK_UP);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_DOWN, SDLK_DOWN);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_ZOOM_IN, SDLK_q);
		client.getUserInputHandler()->setStateCmdKey(Cmds::TMP_ZOOM_OUT, SDLK_e);


		client.getUserInputHandler()->aimMode = UserInputHandler::KEYBOARD;
	
		skyBox.init();
		client.init();
		gui.init(this);
	}

	void Game::startServer(int clients)
	{
		if (runningServer)
			return;		// Server already started
		
		runningServer = true;
		numberOfClients = clients;
		serverThread = SDL_CreateThread(runServer, 0);

		if (serverThread == 0)
		{
			fprintf(stderr, "Unable to create server thread: %s\n", SDL_GetError());
	        exit(-1);
		}
	}

	bool Game::connectToServer(std::string &host)
	{
		if (connectedToServer)
			return false;		// already connected to server
	
		MessageSender *sender;
		MessageReciever *reciever;
	
		if (CLIENT_PRINT_NETWORK_DEBUG) printf("CLIENT: Trying to connect to server\n");

		// Connect to server
		bool connected = networkClient.openConnection(host);
		
		sender = networkClient.getMessageSender();
		reciever = networkClient.getMessageReciever();
		client.setConnection(sender, reciever);
		client.setColor(Color(0.0f, 1.0f, 0.0f));
		
		// Initialize
		while (running && connected)
		{
			if (CLIENT_PRINT_NETWORK_DEBUG) printf("CLIENT: Initializing\n");

			pollEvents();
		
			bool initialized = client.initConnection();

			if (initialized)
			{
				connectedToServer = true;
				break;
			}
				
			SDL_Delay(5);
		}
		
		return connectedToServer;
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
			gui.pushInput(event);
		}		
	}
};

#include "Game.h"
#include "vec3f.h"
#include "TimeHandler.h"
#include "NetworkConnection.h"
#include "Cmds.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

//#define USE_VIRTUAL_CONNECTION

namespace Prototype
{
	bool SHOW_CLIENT_1 = false;
	bool SHOW_CLIENT_2 = false;
	bool SHOW_SERVER = false;

	const Vec2<int> Game::WINDOW_SIZE = Vec2<int>(960, 480);

	Game::Game()
	{
		init();
		running = true;

		if (configHandler.loadFile(CONFIG_FILENAME))
		{
			std::cout << "Game: using config file for game parameters" << std::endl;
		}
		else
		{
			std::cout << "* * * Game: Config file not found, using default values!!! * * *" << std::endl;
		}
	}

	Game::~Game()
	{
		SDL_Quit( );
		SDLNet_Quit();
	}

	void Game::run(std::string &host)
	{
		client1.getTimeHandler()->reset();
		client2.getTimeHandler()->reset();
		server.getTimeHandler()->reset();

		server.getTimeHandler()->incrementTime(1234);

		printf("Choose.\n");
		
		while (true)
		{
			pollEvents();
		
			if (client1.getKeyHandler()->isPressed(CMD_UP))
			{
				SHOW_SERVER = true;
				SHOW_CLIENT_1 = true;
				SHOW_CLIENT_2 = true;
				break;
			}
		
			if (client1.getKeyHandler()->isPressed(CMD_SHOOT))
			{
				SHOW_SERVER = true;
				SHOW_CLIENT_1 = true;
				client1.getUserInputHandler()->aimMode = UserInputHandler::MOUSE;
				client1.getUserInputHandler()->setActionCmdKey(Cmds::START_SHOOTING, Cmds::STOP_SHOOTING, SDL_BUTTON_LEFT);		
				break;
			}
			
			if (client1.getKeyHandler()->isPressed(CMD_LEFT))
			{
				SHOW_CLIENT_1 = true;
				client1.getUserInputHandler()->aimMode = UserInputHandler::MOUSE;
				client1.getUserInputHandler()->setActionCmdKey(Cmds::START_SHOOTING, Cmds::STOP_SHOOTING, SDL_BUTTON_LEFT);		
				break;
			}

			if (client1.getKeyHandler()->isPressed(CMD_DOWN))
			{
				SHOW_SERVER = true;
				printf("SHOW_SERVER = true\n");
				break;
			}

			if (client1.getKeyHandler()->isPressed(CMD_RIGHT))
			{
				SHOW_CLIENT_2 = true;
				break;
			}
				
			SDL_Delay(20);
		}
		
		#ifndef USE_VIRTUAL_CONNECTION
		NetworkServer networkServer;
		NetworkClient networkClient1;
		NetworkClient networkClient2;
		#endif
		
		size_t clientsConnected = 0;
		bool client1Connected = false;
		bool client2Connected = false;
		#ifndef USE_VIRTUAL_CONNECTION
		if (SHOW_SERVER)
			networkServer.start();
		
		NetworkServerClient *serverClient1;
		NetworkServerClient *serverClient2;
		
		// -------------------------------------------------------
		// INITIALIZE NETWORK
		// -------------------------------------------------------
		while (true)
		{
			if (SHOW_CLIENT_1)
			{
				// client 1
				if (!client1Connected)
					client1Connected |= networkClient1.openConnection(host);
			}
			
			if (SHOW_CLIENT_2)
			{
				// client 2
				if (!client2Connected)
					client2Connected |= networkClient2.openConnection(host);
			}

			if (SHOW_SERVER)
			{
				// server
				NetworkServerClient *serverClient = networkServer.checkForNewClient();

				if (clientsConnected == 0)
					serverClient1 = serverClient;
					
				if (clientsConnected == 1)
					serverClient2 = serverClient;

				if (serverClient != 0)
					clientsConnected++;
					
					
				printf("waiting for client: %d\n", clientsConnected);
			}
			
			if ((!SHOW_CLIENT_1 || client1Connected) 
				&& (!SHOW_CLIENT_2 || client2Connected) 
				&& (!SHOW_SERVER || clientsConnected == 2))
				break;
				
				
			SDL_Delay(20);
		}
		#endif

		MessageSender *sender1;
		MessageReciever *reciever1;
		MessageSender *sender3;
		MessageReciever *reciever3;
		MessageSender *sender2;
		MessageReciever *reciever2;
		MessageSender *sender4;
		MessageReciever *reciever4;

		#ifndef USE_VIRTUAL_CONNECTION		
		if (SHOW_CLIENT_1)
		{
			sender1 = networkClient1.getMessageSender();
			reciever1 = networkClient1.getMessageReciever();
			sender1->v = 1;
			reciever1->v = 1;
		}
		if (SHOW_CLIENT_2)
		{
			sender3 = networkClient2.getMessageSender();
			reciever3 = networkClient2.getMessageReciever();
			sender3->v = 3;
			reciever3->v = 3;
		}
		if (SHOW_SERVER)
		{
			sender2 = &(serverClient1->sender);
			reciever2 = &(serverClient1->reciever);
			sender2->v = 2;
			reciever2->v = 2;

			sender4 = &(serverClient2->sender);
			reciever4 = &(serverClient2->reciever);
			sender4->v = 4;
			reciever4->v = 4;
		}
		#endif
		
		#ifdef USE_VIRTUAL_CONNECTION

		// Initialize virtual connections
		sender1 = virtualConnection1.getMessageSender();
		reciever2 = virtualConnection1.getMessageReciever();

		sender2 = virtualConnection2.getMessageSender();
		reciever1 = virtualConnection2.getMessageReciever();

		sender3 = virtualConnection3.getMessageSender();
		reciever4 = virtualConnection3.getMessageReciever();

		sender4 = virtualConnection4.getMessageSender();
		reciever3 = virtualConnection4.getMessageReciever();
		#endif


		// set the init simulated lag
		int simulatedLag = 0;
		if (SHOW_CLIENT_1)
		{
			simulatedLag = client1.getConfigHandler().getIntValue("server_to_client_1_init_simulated_lag",
																	SERVER_TO_CLIENT_1_INIT_SIMULATED_LAG_DEFAULT);
			reciever1->setSimulatedLag(simulatedLag);
		}
		if (SHOW_CLIENT_2)
		{
			simulatedLag = client2.getConfigHandler().getIntValue("server_to_client_2_init_simulated_lag",
																	SERVER_TO_CLIENT_2_INIT_SIMULATED_LAG_DEFAULT);
			reciever3->setSimulatedLag(simulatedLag);
		}
		if (SHOW_SERVER)
		{
			simulatedLag = client1.getConfigHandler().getIntValue("client_1_to_server_init_simulated_lag",
																	CLIENT_1_TO_SERVER_INIT_SIMULATED_LAG_DEFAULT);
			reciever2->setSimulatedLag(simulatedLag);

			simulatedLag = client2.getConfigHandler().getIntValue("client_2_to_server_init_simulated_lag",
																	CLIENT_2_TO_SERVER_INIT_SIMULATED_LAG_DEFAULT);
			reciever4->setSimulatedLag(simulatedLag);
		}




		
		if (SHOW_CLIENT_1)
		{
			client1.setConnection(sender1, reciever1);
			client1.setColor(Color(0.0f, 1.0f, 0.0f));
		}
		
		if (SHOW_CLIENT_2)
		{
			client2.setConnection(sender3, reciever3);
			client2.setColor(Color(1.0f, 0.0f, 0.0f));
		}

		
		client1Connected = false;
		client2Connected = false;
		clientsConnected = 0;
		
		printf("TIME BEFORE SYNC serverT: %d, client1T: %d\n", server.getTimeHandler()->getTime(), client1.getTimeHandler()->getTime());
	
		while (true)
		{
			// --------------
			// Loop at server 
			// Run clientConnected when a new connection is detected. (Here we simulate that two clients is detected)
			// --------------
			if (SHOW_SERVER)
			{
				if (server.clientConnected(sender2, reciever2))
					clientsConnected++;
				
				if (server.clientConnected(sender4, reciever4))
					clientsConnected++;
			}

			// --------------
			// Loop at client 1
			// --------------
			if (SHOW_CLIENT_1)
				client1Connected |= client1.initConnection();

			// --------------
			// Loop at client 2
			// --------------
			if (SHOW_CLIENT_2)
				client2Connected |= client2.initConnection();
			
			
			// Break if all is connected that should be
			if ((!SHOW_CLIENT_1 || client1Connected) 
				&& (!SHOW_CLIENT_2 || client2Connected) 
				&& (!SHOW_SERVER || clientsConnected == 2))
				break;

			SDL_Delay(1);
		}
		printf("TIME AFTER SYNC serverT: %d, client1T: %d\n", server.getTimeHandler()->getTime(), client1.getTimeHandler()->getTime());
		
		
		// set the final simulated lag
		if (SHOW_CLIENT_1)
		{
			simulatedLag = client1.getConfigHandler().getIntValue("server_to_client_1_final_simulated_lag",
																	SERVER_TO_CLIENT_1_FINAL_SIMULATED_LAG_DEFAULT);
			reciever1->setSimulatedLag(simulatedLag);
		}
		if (SHOW_CLIENT_2)
		{
			simulatedLag = client2.getConfigHandler().getIntValue("server_to_client_2_final_simulated_lag",
																	SERVER_TO_CLIENT_2_FINAL_SIMULATED_LAG_DEFAULT);
			reciever3->setSimulatedLag(simulatedLag);
		}
		if (SHOW_SERVER)
		{
			simulatedLag = client1.getConfigHandler().getIntValue("client_1_to_server_final_simulated_lag",
																	CLIENT_1_TO_SERVER_FINAL_SIMULATED_LAG_DEFAULT);
			reciever2->setSimulatedLag(simulatedLag);

			simulatedLag = client2.getConfigHandler().getIntValue("client_2_to_server_final_simulated_lag",
																	CLIENT_2_TO_SERVER_FINAL_SIMULATED_LAG_DEFAULT);
			reciever4->setSimulatedLag(simulatedLag);
		}
		


		server.startGame();

		while (running) 
		{
			pollEvents();

			if (SHOW_CLIENT_1)
			{
				client1.runStep();
			}
				
			if (SHOW_CLIENT_2)
			{
				client2.runStep();
			}

			if (SHOW_SERVER)
			{
				for (int i = 0; i < 3; i++)
					server.logic();
			}

			// RENDER
			if (((client1.getRequestRender() || !SHOW_CLIENT_1) &&
				(client2.getRequestRender() || !SHOW_CLIENT_2) // &&
				) ||
				(configHandler.getIntValue("render_at_least_every_tick", RENDER_AT_LEAST_EVERY_TICK_DEFAULT) == 1))
			{
				render(0);
			}

			if (USE_DELAY) SDL_Delay(1);
		}

		#ifndef USE_VIRTUAL_CONNECTION
		networkServer.close();
		networkClient1.close();
		networkClient2.close();
		#endif
	}

	void Game::render(Uint32 time)
	{
		glDisable(GL_LIGHTING);

		if (SHOW_CLIENT_1)
		{
			client1.useViewport();
			client1.renderAndUpdate();
		}

		if (SHOW_CLIENT_2)
		{
			client2.useViewport();
			client2.renderAndUpdate();
		}

		if (SHOW_SERVER)
		{
			server.useViewport();
			server.render();
		}
		
		glViewport(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y); 

		draw(time);
		
		glEnable(GL_LIGHTING);
		glFlush();
		SDL_GL_SwapBuffers();

		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Game::draw(Uint32 time)
	{
		// Draw x, y, z axis in red, green, blue
		glDisable(GL_LIGHTING);
		glPushMatrix();
			glScalef(50.0, 50.0, 1.0);
			glBegin(GL_LINES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(5.0f, 0.0f, 0.0f);
			glEnd();
			glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 5.0f, 0.0f);
			glEnd();
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 5.0f);
			glEnd();
		glPopMatrix();
		glEnable(GL_LIGHTING);
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

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// ------- set viewports -----------		
		{
			static const uint BORDER_WIDTH = 1;
			uint w2 = w - BORDER_WIDTH*2;

			server.setViewport(0, h/4, w2/3, (h*3)/4);
			client1.setViewport(w2/3 + BORDER_WIDTH, h/4, w2/3, (h*3)/4);
			client2.setViewport(((w2*2)/3) + BORDER_WIDTH*2, h/4, w2/3, (h*3)/4);
		}

		client1.getUserInputHandler()->setStateCmdKey(Cmds::LEFT, SDLK_a);
		client1.getUserInputHandler()->setStateCmdKey(Cmds::RIGHT, SDLK_d);
		client1.getUserInputHandler()->setStateCmdKey(Cmds::FORWARD, SDLK_w);
		client1.getUserInputHandler()->setStateCmdKey(Cmds::BACKWARD, SDLK_s);
		client1.getUserInputHandler()->setStateCmdKey(Cmds::ROTATE_LEFT, SDLK_c);
		client1.getUserInputHandler()->setStateCmdKey(Cmds::ROTATE_RIGHT, SDLK_v);
		client1.getUserInputHandler()->setActionCmdKey(Cmds::START_SHOOTING, Cmds::STOP_SHOOTING, SDLK_SPACE);
		client1.getUserInputHandler()->setActionCmdKey(Cmds::SWITCH_WEAPON, SDLK_x);

		client2.getUserInputHandler()->setStateCmdKey(Cmds::LEFT, SDLK_j);
		client2.getUserInputHandler()->setStateCmdKey(Cmds::RIGHT, SDLK_l);
		client2.getUserInputHandler()->setStateCmdKey(Cmds::FORWARD, SDLK_i);
		client2.getUserInputHandler()->setStateCmdKey(Cmds::BACKWARD, SDLK_k);
		client2.getUserInputHandler()->setActionCmdKey(Cmds::START_SHOOTING, Cmds::STOP_SHOOTING, SDL_BUTTON_LEFT);		
		client2.getUserInputHandler()->setActionCmdKey(Cmds::SWITCH_WEAPON, SDLK_u);
	
	
		client1.getKeyHandler()->setClient1Keys();
		client2.getKeyHandler()->setClient2Keys();

		// default aim mode
		client1.getUserInputHandler()->aimMode = UserInputHandler::KEYBOARD;
		client2.getUserInputHandler()->aimMode = UserInputHandler::MOUSE;
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
				client1.getKeyHandler()->setKeyPressed(event.key.keysym.sym);
				client2.getKeyHandler()->setKeyPressed(event.key.keysym.sym);
			
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;				   
				case SDLK_f:					
					if (event.key.keysym.mod & KMOD_CTRL)
						SDL_WM_ToggleFullScreen(screen);
					break;
				default:
					break;
				} // end switch
				break;
			}
			
			case SDL_KEYUP:
				client1.getKeyHandler()->setKeyReleased(event.key.keysym.sym);
				client2.getKeyHandler()->setKeyReleased(event.key.keysym.sym);
			
				break;
				
			case SDL_QUIT:
				running = false;
		  break;

			default:
				break;
	     
			} // end switch
			
			client1.getUserInputHandler()->pushInput(event);
			client2.getUserInputHandler()->pushInput(event);

		} // end while
		
	}
};

#include "Game.h"
#include "vec3f.h"
#include "TimeHandler.h"
#include "NetworkConnection.h"
#include "Cmds.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

namespace Prototype
{
	bool SHOW_CLIENT_1 = false;
	bool SHOW_CLIENT_2 = false;
	bool SHOW_SERVER = false;

	const Vec2<int> Game::WINDOW_SIZE = Vec2<int>(960, 480);

	Game::Game()
	{
		init();
		//gui.init();
		running = true;

		//debugOutput = std::cout;
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
			
			if (client1.getKeyHandler()->isPressed(CMD_LEFT))
			{
				SHOW_CLIENT_1 = true;
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
		
		
		//SHOW_SERVER = true;
		//SHOW_CLIENT_1 = true;
		//SHOW_CLIENT_2 = true;

		NetworkServer networkServer;
		NetworkClient networkClient1;
		NetworkClient networkClient2;

		size_t clientsConnected = 0;
		bool client1Connected = false;
		bool client2Connected = false;

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


		MessageSender *sender1;
		MessageReciever *reciever1;
		MessageSender *sender3;
		MessageReciever *reciever3;
		MessageSender *sender2;
		MessageReciever *reciever2;
		MessageSender *sender4;
		MessageReciever *reciever4;


		if (SHOW_CLIENT_1)
		{
			sender1 = networkClient1.getMessageSender();
			reciever1 = networkClient1.getMessageReciever();
			reciever1->setSimulatedLag(SERVER_TO_CLIENT_1_SIMULATED_LAG);
			sender1->v = 1;
			reciever1->v = 1;
		}
		if (SHOW_CLIENT_2)
		{
			sender3 = networkClient2.getMessageSender();
			reciever3 = networkClient2.getMessageReciever();
			reciever3->setSimulatedLag(SERVER_TO_CLIENT_2_SIMULATED_LAG);
			sender3->v = 3;
			reciever3->v = 3;
		}
		if (SHOW_SERVER)
		{
			sender2 = &(serverClient1->sender);
			reciever2 = &(serverClient1->reciever);
			reciever2->setSimulatedLag(CLIENT_1_TO_SERVER_SIMULATED_LAG);
			sender2->v = 2;
			reciever2->v = 2;

			sender4 = &(serverClient2->sender);
			reciever4 = &(serverClient2->reciever);
			reciever4->setSimulatedLag(CLIENT_2_TO_SERVER_SIMULATED_LAG);
			sender4->v = 4;
			reciever4->v = 4;
		}

		
		//timeHandler.reset();


		/*
		// Initialize virtual connections
		sender1 = virtualConnection1.getMessageSender();
		reciever2 = virtualConnection1.getMessageReciever();
		reciever2->setSimulatedLag(CLIENT_1_TO_SERVER_SIMULATED_LAG);

		sender2 = virtualConnection2.getMessageSender();
		reciever1 = virtualConnection2.getMessageReciever();
		reciever1->setSimulatedLag(SERVER_TO_CLIENT_1_SIMULATED_LAG);

		sender3 = virtualConnection3.getMessageSender();
		reciever4 = virtualConnection3.getMessageReciever();
		reciever4->setSimulatedLag(CLIENT_2_TO_SERVER_SIMULATED_LAG);

		sender4 = virtualConnection4.getMessageSender();
		reciever3 = virtualConnection4.getMessageReciever();
		reciever3->setSimulatedLag(SERVER_TO_CLIENT_2_SIMULATED_LAG);
		*/

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
				//if (!client1Connected)
					if (server.clientConnected(sender2, reciever2))
						clientsConnected++;
				
				//if (!client2Connected)
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
		
		server.startGame();

		while (running) 
		{
			//printf("serverT: %d, clientT: %d\n", server.getTimeHandler()->getTime(), client1.getTimeHandler()->getTime());

			pollEvents();

			if (SHOW_CLIENT_1)
			{
				//nowRunning = "client1 logic: ";
				//std::cout << nowRunning;
				client1.runStep();
				//std::cout << std::endl;
			}
				
			if (SHOW_CLIENT_2)
			{
				//nowRunning = "client2 logic: ";
				//std::cout << nowRunning;
				client2.runStep();
				//std::cout << std::endl;
				
			}

			if (SHOW_SERVER)
			{
				//nowRunning = "server logic: ";
				//std::cout << nowRunning;
				for (int i = 0; i < 3; i++)
					server.logic();
				//std::cout << std::endl;
			}
			// guichan
			//gui.gui->logic();

			// RENDER			
			if (client1.getRequestRender() &&
				client2.getRequestRender() &&
				server.getRequestRender())
			{
				render(0);
			}
			//render(0);

			if (USE_DELAY) SDL_Delay(1);
		}

		networkServer.close();
		networkClient1.close();
		networkClient2.close();
	}

	void Game::render(Uint32 time)
	{
		//glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_LIGHTING);

		if (SHOW_CLIENT_1)
		{
			//nowRunning = "client1 render: ";
			client1.useViewport();
			client1.renderAndUpdate();
			//std::cout << std::endl;
		}

		if (SHOW_CLIENT_2)
		{
			//nowRunning = "client2 render: ";
			client2.useViewport();
			client2.renderAndUpdate();
			//std::cout << std::endl;
		}

		if (SHOW_SERVER)
		{
			//nowRunning = "server render: ";
			server.useViewport();
			server.render();
			//std::cout << std::endl;
		}
		
		// guichan
		glViewport(0, 0, WINDOW_SIZE.x, WINDOW_SIZE.y); 
		//gui.gui->draw();


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
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// ------- set viewports -----------		
		{
			static const uint BORDER_WIDTH = 1;
			uint w2 = w - BORDER_WIDTH*2;
			
			client1.setViewport(0, h/4, w2/3, (h*3)/4);
			client2.setViewport(w2/3 + BORDER_WIDTH, h/4, w2/3, (h*3)/4);
			server.setViewport(((w2*2)/3) + BORDER_WIDTH*2, h/4, w2/3, (h*3)/4);
		}

		

		//client1.viewportHandler.screenRenderPos = Vec2<int>(0, h/4);
		//viewportHandler1.screenRenderSize = Vec2<int>(w/2, (h*3)/4);
		//viewportHandler2.screenRenderPos = Vec2<int>(w/2, h/4);
		//viewportHandler2.screenRenderSize = viewportHandler1.screenRenderSize;
		
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
			
			case SDL_MOUSEMOTION:
				{

					//Vec2<int> mouseScreenPos(event.motion.x, event.motion.y);
					//mouseScreenPos.y = WINDOW_SIZE.y - mouseScreenPos.y; // convert from SDL to GL position
					//client2.setCurrentMousePos(mouseScreenPos);
				}
			break;
				break;
			case SDL_QUIT:
				running = false;
		  break;

			default:
				break;
	     
			} // end switch
			

			client1.getUserInputHandler()->pushInput(event);
			client2.getUserInputHandler()->pushInput(event);

			// Pass event to guichan
			//gui.input->pushInput(event);
		} // end while
		
	}
};










// -------------------------------------------------------------------
// DEBUG CODE
// -------------------------------------------------------------------

		/*
		Link clientLink(networkClient2.getMessageSender(), networkClient2.getMessageReciever());
		
		ShootCmd shootCmd(3, 4);
		
		Color color(1.0f, 0.5f, 0.3f);
		InitClient initClient(color);
		
		UserCmd userCmd;
		userCmd.cmdUp = true;
		userCmd.cmdDown = false;
		userCmd.cmdRight = false;
		userCmd.cmdLeft = true;
		userCmd.viewangle = 0.5f;

		clientLink.pushMessage(shootCmd);
		clientLink.pushMessage(userCmd);
		clientLink.pushMessage(initClient);
		clientLink.transmit();
		
		SDL_Delay(10);
		

		Link serverLink(&(serverClient->sender), &(serverClient->reciever));

		if (serverLink.hasMessageOnQueue())
		{
			serverLink.popMessage();
			ShootCmd *shootCmd2 = serverLink.getPoppedData<ShootCmd>();
			printf("playerId: %d, weapon: %d\n", shootCmd2->playerId, shootCmd2->weapon);
		}

		if (serverLink.hasMessageOnQueue())
		{
			serverLink.popMessage();
			UserCmd *userCmd2 = serverLink.getPoppedData<UserCmd>();
			printf("usercmd: up: %d, down: %d\n", userCmd2->cmdUp, userCmd2->cmdDown);
		}
		
		if (serverLink.hasMessageOnQueue())
		{
			serverLink.popMessage();
			InitClient *initClient2 = serverLink.getPoppedData<InitClient>();
			printf("color: %f, %f, %f\n", initClient2->color.r, initClient2->color.g, initClient2->color.b);
		}
 		*/

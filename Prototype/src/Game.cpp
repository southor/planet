#include "Game.h"
#include "vec3f.h"
#include "TimeHandler.h"
#include "NetworkConnection.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

	namespace Prototype
{
	const Vec2<int> Game::WINDOW_SIZE = Vec2<int>(640, 480);

	Game::Game()
	{
		init();
		gui.init();
		running = true;
	}

	Game::~Game()
	{
		SDL_Quit( );
		SDLNet_Quit();
	}

	void Game::run()
	{
		/*
		NetworkServer networkServer;
		NetworkClient networkClient1;
		NetworkClient networkClient2;

		size_t clientsConnected = 0;
		
		networkServer.start();
		
		NetworkServerClient *serverClient;
		while (clientsConnected < 2)
		{
			if (clientsConnected == 0)
				networkClient1.openConnection();
			
			if (clientsConnected == 1)
				networkClient2.openConnection();

			serverClient = networkServer.checkForNewClient();
			if (serverClient != 0)
			{
				clientsConnected++;
			}
		}

		MessageSender *sender = networkClient2.getMessageSender();
		
		ShootCmd shootCmd(3, 4);
		Message message;
		message.type = 42;
		message.time = 1234;
		message.data = &shootCmd;
		
		sender->pushMessage(message);
		sender->transmit();
		
		SDL_Delay(10);
		
		MessageReciever *reciever = &(serverClient->reciever);

		if (reciever->hasMessageOnQueue())
		{
			Message message2 = reciever->popMessage();
			printf("Message recieved, type: %d, ", message2.type);

			ShootCmd *shootCmd2 = (ShootCmd*)message2.data;
			printf("playerId: %d, weapon: %d\n", shootCmd2->playerId, shootCmd2->weapon);

			delete shootCmd2;
		}
 		

		networkServer.close();
		networkClient1.close();
		networkClient2.close();
		*/
		
		

		
		//timeHandler.reset();

		client1.getKeyHandler()->setClient1Keys();
		client2.getKeyHandler()->setClient2Keys();
		client2.setAimMode(Client::MOUSE);


		// Initialize virtual connections
		MessageSender *sender1 = virtualConnection1.getMessageSender();
		MessageReciever *reciever2 = virtualConnection1.getMessageReciever();
		reciever2->setSimulatedLag(50);

		MessageSender *sender2 = virtualConnection2.getMessageSender();
		MessageReciever *reciever1 = virtualConnection2.getMessageReciever();
		reciever1->setSimulatedLag(50);

		MessageSender *sender3 = virtualConnection3.getMessageSender();
		MessageReciever *reciever4 = virtualConnection3.getMessageReciever();
		reciever4->setSimulatedLag(50);

		MessageSender *sender4 = virtualConnection4.getMessageSender();
		MessageReciever *reciever3 = virtualConnection4.getMessageReciever();
		reciever3->setSimulatedLag(50);

		client1.setConnection(sender1, reciever1);
		client1.setColor(Color(0.0f, 1.0f, 0.0f));
		
		client2.setConnection(sender3, reciever3);
		client2.setColor(Color(1.0f, 0.0f, 0.0f));


		
		bool client1Connected = false;
		bool client2Connected = false;
		
		while (!client1Connected || !client2Connected)
		{
			// --------------
			// Loop at server 
			// Run clientConnected when a new connection is detected. (Here we simulate that two clients is detected)
			// --------------
			if (!client1Connected)
				server.clientConnected(sender2, reciever2);
			
			if (!client2Connected)
				server.clientConnected(sender4, reciever4);

			// --------------
			// Loop at client 1
			// --------------
			client1Connected |= client1.initConnection();

			// --------------
			// Loop at client 2
			// --------------
			client2Connected |= client2.initConnection();
		}
		

/*
		// ----------------------------------------------------
		// NOT NEEDED ANY MORE. IS KEPT HERE FOR DEBUG PURPOSE.
		// ----------------------------------------------------

		Color color1 = Color(0.0f, 1.0f, 0.0f);							// handle by packet
		Color color2 = Color(1.0f, 0.0f, 0.0f);							// handle by packet
		Pos startPos(200.0f, 200.0f);									// handle by packet
		Pos startPos2(200.0f, 250.0f);									// handle by packet

		Link link = Link(sender1, reciever2);
		size_t playerId = server.addClient(color1, link);
		server.addPlayerObj(playerId, startPos);						// handle by packet
		client1.setPlayerId(playerId);									// handle by packet

		Link link2 = Link(sender3, reciever4);
		size_t playerId2 = server.addClient(color2, link2);
		server.addPlayerObj(playerId2, startPos2);						// handle by packet
		client2.setPlayerId(playerId2);									// handle by packet

		// rendering
		client1.addPlayer(color1, startPos);							// handle by packet
		client1.addPlayer(color2, startPos2);							// handle by packet
		client2.addPlayer(color1, startPos);							// handle by packet
		client2.addPlayer(color2, startPos2);							// handle by packet
*/


		server.startGame();

		while (running) 
		{
			pollEvents();

			client1.logic();
			client2.logic();

			server.logic();
			
			// guichan
			gui.gui->logic();

			// RENDER
			render(0.0f);

			SDL_Delay(20);
		}
	}

	void Game::render(Uint32 time)
	{
		//glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_LIGHTING);

		//viewportHandler1.setupViewport();
		//glViewport(0, h/4, w/2, (h*3)/4);
		client1.useViewport();
		client1.render();

		//viewportHandler.screenRenderPos = Vec2<int>(w/2, h/4);
		//viewportHandler.screenRenderSize = Vec2<int>(w/2, (h*3)/4);
		//glViewport(w/2, h/4, w/2, (h*3)/4);
		//viewportHandler2.setupViewport();
		client2.useViewport();
		client2.render();

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
		glDisable(GL_LIGHTING);
		
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

		
		unsigned int flags = SDL_HWSURFACE | SDL_OPENGL; // |SDL_FULLSCREEN;
		unsigned int w = WINDOW_SIZE.x;
		unsigned int h = WINDOW_SIZE.y;

		unsigned int bpp = 32;	

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
		client1.setViewport(0, h/4, w/2, (h*3)/4);
		client2.setViewport(w/2, h/4, w/2, (h*3)/4);

		

		//client1.viewportHandler.screenRenderPos = Vec2<int>(0, h/4);
		//viewportHandler1.screenRenderSize = Vec2<int>(w/2, (h*3)/4);
		//viewportHandler2.screenRenderPos = Vec2<int>(w/2, h/4);
		//viewportHandler2.screenRenderSize = viewportHandler1.screenRenderSize;
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
			
				switch(event.key.keysym.sym)
				{
				default:
					break;
				}
				break;
			
			case SDL_MOUSEMOTION:
				{
					//xrel = event.motion.xrel;
					//yrel = event.motion.yrel;
					//printf("Mouse moved by %d,%d to (%d,%d)\n", 
					//event.motion.xrel, event.motion.yrel,
					//event.motion.x, event.motion.y);
					Vec2<int> mouseScreenPos(event.motion.x, event.motion.y);
					mouseScreenPos.y = WINDOW_SIZE.y - mouseScreenPos.y; // convert from SDL to GL position
					client2.setCurrentMousePos(mouseScreenPos);
				}
			break;
				break;
			case SDL_QUIT:
				running = false;
		  break;

			default:
				break;
	     
			} // end switch

			// Pass event to guichan
			gui.input->pushInput(event);
		} // end while
		
	}
};
#include "game.h"
#include "vec3f.h"
#include "TimeHandler.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

namespace Prototype
{
	unsigned int w;
	unsigned int h;

	Game::Game()
	{
		init();
		//gui.init();
		running = true;
	}

	Game::~Game()
	{
		SDL_Quit( );
	}

	Uint32 Game::getTicks()
	{
	  return SDL_GetTicks() - mStartTime;
	}

	void Game::run()
	{
		Uint32 lastTime = getTicks();
		mStartTime = SDL_GetTicks();
		timeHandler.reset();

		// NETWORK
		Message message;
		message.type = 5;
		std::string test("test");
		message.data = &test;
		
		Message message2;
		message2.type = 8;
		std::string other("other");
		message2.data = &other;
		
		MessageSender *sender1 = virtualConnection1.getMessageSender();
		MessageReciever *reciever1 = virtualConnection1.getMessageReciever();

		MessageSender *sender2 = virtualConnection2.getMessageSender();
		MessageReciever *reciever2 = virtualConnection2.getMessageReciever();

		MessageSender *sender3 = virtualConnection3.getMessageSender();
		MessageReciever *reciever3 = virtualConnection3.getMessageReciever();

		MessageSender *sender4 = virtualConnection4.getMessageSender();
		MessageReciever *reciever4 = virtualConnection4.getMessageReciever();

		client1.getKeyHandler()->setClient1Keys();
		client2.getKeyHandler()->setClient2Keys();

		client1.setConnection(sender2, reciever1);
		client2.setConnection(sender4, reciever3);

		Server server;
		//server.addClient(sender1, reciever2);
		Pos startPos(200.0f, 200.0f);
		Pos startPos2(200.0f, 250.0f);

		Color color1 = Color(1.0f, 0.0f, 0.0f);
		size_t playerId = server.addClient(color1, sender1, reciever2);
		server.addPlayerObj(playerId, startPos);
		client1.setPlayerId(playerId);

		Color color2 = Color(0.0f, 1.0f, 0.0f);
		size_t playerId2 = server.addClient(color2, sender3, reciever4);
		server.addPlayerObj(playerId2, startPos2);
		client2.setPlayerId(playerId2);

		// rendering
		client1.addPlayer(color1, startPos);
		client1.addPlayer(color2, startPos2);
		client2.addPlayer(color1, startPos);
		client2.addPlayer(color2, startPos2);

		server.startGame();

		while (running) 
		{
			pollEvents();

			Uint32 time = getTicks();      

			/*
			int diff = (int)(time-lastTime);		
			float diffInSec = diff/1000.0f;
			float speed = 2*5.5556f; // 5.5556 m/s = 20 km/h
			*/


			
			/*
			if (kh.getPressed(CMD_LEFT))
			{
				client.sendMessage(message);
				client.sendMessage(message2);
			}
			*/

			client1.logic();
			client2.logic();

			server.logic();
			
			// guichan
			//gui.gui->logic();


			// RENDER
			render(time);

//#ifdef _WIN32
//		 Sleep(100);
//#else
		 SDL_Delay(20);
//#endif

			lastTime = time;
		}
	}

	void Game::render(Uint32 time)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);


		// rendering the same client to both render areas
		glDisable(GL_LIGHTING);

		glViewport(0, h/4, w/2, h*3/4);
		client1.render();

		glViewport(w/2, h/4, w/2, h*3/4);
		client2.render();

		// guichan
		glViewport(0, 0, w, h);
		//gui.gui->draw();



		draw(time);

		glEnable(GL_LIGHTING);
		glFlush();
		SDL_GL_SwapBuffers();
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

		if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError() );
		SDL_Quit( );
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
		//w = 1024;
		//h = 768;
		w = 640;
		h = 480;

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
				//xrel = event.motion.xrel;
				//yrel = event.motion.yrel;
				//printf("Mouse moved by %d,%d to (%d,%d)\n", 
				//event.motion.xrel, event.motion.yrel,
				//event.motion.x, event.motion.y);
				break;
			case SDL_QUIT:
				running = false;
		  break;

			default:
				break;
	     
			} // end switch

			// Pass event to guichan
			//gui.input->pushInput(event);
		} // end while
		
	}
};
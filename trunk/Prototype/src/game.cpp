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
		timeHandler.setStartTime(mStartTime);

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

		//Client client(sender2, reciever1);
		client.setConnection(sender2, reciever1);
		client.setKeyHandler(&kh);

		Server server;
		server.addClient(sender1, reciever2);

		// rendering
		client.addPlayer(Color(0.0f, 0.0f, 1.0f), Pos(200.0f, 200.0f));

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

			client.logic();

			server.logic();
			
			client.recieveMessages();
			
			// RENDER
			render(time);

#ifdef _WIN32
		 Sleep(100);
#else
		 sleep(0.1);
#endif

			lastTime = time;
		}
	}

	void Game::render(Uint32 time)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);


		// rendering the same client to both render areas

		glViewport(0, h/4, w/2, h*3/4);
		//glDisable(GL_LIGHTING);		
		draw(time);

		glViewport(w/2, h/4, w/2, h*3/4);
		draw(time);

		glFlush();
		SDL_GL_SwapBuffers();
	}

	void Game::draw(Uint32 time)
	{
		glDisable(GL_LIGHTING);
		if (!kh.isDown(CMD_LEFT))
		{
			//glBegin(GL_TRIANGLES);
			//	//glNormal3f(0.0f, 0.0f, 1.0f);
			//	glColor3f(1.0f,0.0f,0.0f);
			//	glVertex3f( 0.0f, 0.3f, 0.0f);
			//	glColor3f(0.0f,1.0f,0.0f);
			//	glVertex3f(-0.3f,-0.3f, 0.0f);
			//	glColor3f(0.0f,0.0f,1.0f);
			//	glVertex3f( 0.3f,-0.3f, 0.0f);
			//glEnd();

			client.render();
		}
		
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
		w = 1024;
		h = 768;
		//	w = 640;
		//	h = 400;

		unsigned int bpp = 32;	

		screen = SDL_SetVideoMode(w, h, bpp, flags);
		if (screen == 0)
		{
			//throw EXCEPTION("Failed to set video mode");
		}

		SDL_ShowCursor(SDL_DISABLE);
		glViewport(0, 0, w, h);

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
				kh.setKeyPressed(event.key.keysym.sym);
			
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
				kh.setKeyReleased(event.key.keysym.sym);
			
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
		} // end while
	}
};
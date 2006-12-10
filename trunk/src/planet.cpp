#include "planet.h"

#include <iostream>
#include <cmath>

#include "SDL_endian.h"

using namespace std;

bool leftkey;
bool rightkey;
bool upkey;
bool downkey;
bool akey;
bool skey;
bool dkey;
bool wkey;

unsigned int w;
unsigned int h;

Planet::Planet()
{
	init();
	running = true;
	renv = 0;

	leftkey = false;
	rightkey = false;
	upkey = false;
	downkey = false;

	cameraXAngle = 0;
	cameraYAngle = 0;
	xrel = 0;
	yrel = 0;
}

Planet::~Planet()
{
	SDL_Quit( );
}
int isFollowingSpline = 1;

Uint32 Planet::getTicks()
{
  return SDL_GetTicks() - mStartTime;
}

void Planet::run()
{
	Uint32 lastTime = getTicks();
	mStartTime = SDL_GetTicks();

	while (running) 
	{
		pollEvents();

		Uint32 time = getTicks();      

		int diff = (int)(time-lastTime);		
		float diffInSec = diff/1000.0f;
		float speed = 2*5.5556f; // 5.5556 m/s = 20 km/h

		if(leftkey) {
			cameraYAngle -= speed*diffInSec*10;
		} else if(rightkey) {
			cameraYAngle += speed*diffInSec*10;
		}
		if(upkey) {
			cameraXAngle -= speed*diffInSec*10;
		} else if(downkey) {
			cameraXAngle += speed*diffInSec*10;
		}

		if (xrel < 100 || yrel < 100) {
			cameraXAngle += yrel/10;
			cameraYAngle += xrel/10;
		}
		xrel = yrel = 0;

		//hjŠlpberŠkningar
		GLfloat m[16];				// modelview matrix
		glPushMatrix();
		glLoadIdentity();
		glRotatef(-cameraYAngle, 0, 1, 0);
		glRotatef(-cameraXAngle, 1, 0, 0);
	  
		GLfloat from[4];
		GLfloat to[4];
	  
		from[0] = 0.0f;
		from[1] = 0.0f;
		from[2] = -1.0f;
		from[3] = 0.0f;
	  
		glGetFloatv(GL_MODELVIEW_MATRIX, m); 
		Vec3f::transformVec(m, from, to);
	  
//		camera0->mLook.x = to[0];
//		camera0->mLook.y = to[1];
//		camera0->mLook.z = to[2];
		glPopMatrix();
		//hjŠlpberŠkningar slut

		if (akey) {
//			camera0->mPos = camera0->mPos + camera0->mLook*(float)(speed*diffInSec);
		} else if (skey) {
//			camera0->mPos = camera0->mPos - camera0->mLook*(float)(speed*diffInSec);
		}

//		camera0->mLook = camera0->mPos + camera0->mLook;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//			glViewport(0, 0, w, h/2);
//			camera0->setFrustumHeightScale(2);

//			glViewport(0, 0, w, h);
//			camera0->setFrustumHeightScale(1);

		
		draw(time);

		glFlush();
		SDL_GL_SwapBuffers();
	  
		//	  Sleep(30);

		lastTime = time;
	}
}




void Planet::draw(Uint32 time)
{
	glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f( 0.0f, 0.3f, 0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(-0.3f,-0.3f, 0.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f( 0.3f,-0.3f, 0.0f);
	glEnd();

	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}


void Planet::init()
{
	running = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	fprintf(stderr, "Video initialization failed: %s\n",
	   SDL_GetError() );
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void Planet::pollEvents()
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

			case SDLK_a:					
				akey = 1;
				break;
			case SDLK_d:					
				dkey = 1;
				break;
			case SDLK_w:					
				wkey = 1;
				break;
			case SDLK_s:					
				skey = 1;
				break;
			case SDLK_LEFT:
				leftkey = true;
				break;
			case SDLK_RIGHT:
				rightkey = true;
				break;
			case SDLK_UP:
				upkey = true;
				break;
			case SDLK_DOWN:
				downkey = true;
				break;
			  
			default:
				break;
			} // end switch
			break;
		}
		case SDL_KEYUP:
			switch(event.key.keysym.sym)
	    {
	    case SDLK_a:					
			akey = 0;
			break;
	    case SDLK_d:					
			dkey = 0;
			break;
	    case SDLK_w:					
			wkey = 0;
			break;
	    case SDLK_s:					
			skey = 0;
			break;
	    case SDLK_LEFT:
			leftkey = false;
			break;
	    case SDLK_RIGHT:
			rightkey = false;
			break;
	    case SDLK_UP:
			upkey = false;
			break;
	    case SDLK_DOWN:
			downkey = false;
			break;
	    default:
			break;
	    }
		break;
		
		case SDL_MOUSEMOTION:
			xrel = event.motion.xrel;
			yrel = event.motion.yrel;
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

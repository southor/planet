#ifndef __planet_h__
#define __planet_h__

#include <vector>

#include "common.h"

class Planet 
{
public:
	Planet();
	~Planet();
	void run();

private:
	void init();

	void draw(Uint32 time);
	void pollEvents();
	Uint32 getTicks();

	bool running;
	SDL_Surface *screen;
	GLfloat renv;

	float xrel;
	float yrel;

	GLfloat cameraXAngle;
	GLfloat cameraYAngle;


	Uint32 mStartTime;
};
#endif

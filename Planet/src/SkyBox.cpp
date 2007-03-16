#include "SkyBox.h"
#include "TextureHandler.h"

#include <string>

namespace Planet
{
	void SkyBox::init()
	{
		textures[0] = TextureHandler::loadTexture("skybox/box1.png");
		textures[1] = TextureHandler::loadTexture("skybox/box2.png");
		textures[2] = TextureHandler::loadTexture("skybox/box3.png");
		textures[3] = TextureHandler::loadTexture("skybox/box4.png");
		textures[4] = TextureHandler::loadTexture("skybox/box5.png");
		textures[5] = TextureHandler::loadTexture("skybox/box6.png");
	}
	
	void SkyBox::render()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
	
		glColor3f(1.0f, 1.0f, 1.0f);
	 
		glPushMatrix();
	 
			glScalef(50.0f, 50.0f, 50.0f);
		
			float r = 1.0f;

			// Y
			glBindTexture(GL_TEXTURE_2D, textures[2]);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);	
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-r, 1.0f, r);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(r, 1.0f, r);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(r, 1.0f, -r); 
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-r, 1.0f, -r);
			glEnd();
		 
			// -Y
			glBindTexture(GL_TEXTURE_2D, textures[3]);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);		
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-r, -1.0f, -r);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(r, -1.0f, -r);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(r, -1.0f, r); 
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-r, -1.0f, r);
			glEnd();
		 
			// -X
			glBindTexture(GL_TEXTURE_2D, textures[1]);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);		
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, r, r);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, r, -r); 
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -r, -r);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -r, r);		
			glEnd();
		 
			// X
			glBindTexture(GL_TEXTURE_2D, textures[0]);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);		
				glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, r, -r);	
				glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f,  r, r); 
				glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f,  -r, r);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -r, -r);
			glEnd();
		 
			// Z
			glBindTexture(GL_TEXTURE_2D, textures[5]);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);		
				glTexCoord2f(0.0f, 0.0f); glVertex3f(r, r, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-r, r, 1.0f); 
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-r, -r, 1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(r, -r, 1.0f);
			glEnd();
		 
			// -Z
			glBindTexture(GL_TEXTURE_2D, textures[4]);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBegin(GL_QUADS);		
				glTexCoord2f(0.0f, 0.0f);  glVertex3f(-r, r, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(r,  r, -1.0f); 
				glTexCoord2f(1.0f, 1.0f); glVertex3f(r,  -r, -1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-r, -r, -1.0f);
			glEnd();	
		
		glPopMatrix();
		
		
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
	}
};


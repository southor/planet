#ifndef __Gui_h__
#define __Gui_h__

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
// A class used to load images for OpenGL using SDL
#include <guichan/opengl/openglsdlimageloader.hpp>


namespace Planet
{
	class Game;

	class NumberOfPlayersListModel : public gcn::ListModel
	{
	public:
		int getNumberOfElements()
		{
			return 4;
		}

		std::string getElementAt(int i)
		{
			switch(i)
			{
				case 0:
					return std::string("1");
				case 1:
					return std::string("2");
				case 2:
					return std::string("3");
				case 3:
					return std::string("4");
				default:
					return std::string("");
			}
		}
	};




	class Gui : public gcn::ActionListener
	{
	public:
		Gui();
		~Gui();
	
		void init(Game *game);
		void initWidgets();

		void render();
		void logic();
		void pushInput(SDL_Event &event);

		void action(const gcn::ActionEvent& actionEvent);

		// Guichan
		gcn::SDLInput* input;                    // Input driver
		gcn::OpenGLGraphics* graphics;           // Graphics driver
		gcn::OpenGLSDLImageLoader* imageLoader;  // For loading images

		/*
		 * Guichan stuff we need
		 */
		gcn::Gui* gui;            // A Gui object - binds it all together
		gcn::ImageFont* font;     // A font

		/*
		 * All of the default widgets
		 */
		gcn::Container* top;

		gcn::Label* labelNumberOfPlayers;
		gcn::Label* labelHost;

		gcn::Button* buttonStartServer;
		gcn::Button* buttonConnectToServer;
		
		gcn::TextField* textFieldHost;

		gcn::DropDown* dropDownNumberOfPlayers;

		gcn::CheckBox* checkBox1;
		gcn::Window *window;
		
		NumberOfPlayersListModel numberOfPlayersListModel;
		
		Game *game;

	};
};

#endif

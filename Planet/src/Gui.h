#ifndef __Gui_h__
#define __Gui_h__

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
// A class used to load images for OpenGL using SDL
#include <guichan/opengl/openglsdlimageloader.hpp>
#include "Color.h"

namespace Planet
{
	class Game;

	class MapListModel : public gcn::ListModel
	{
	public:
		int getNumberOfElements()
		{
			return 2;
		}

		std::string getElementAt(int i)
		{
			switch(i)
			{
				case 0:
					return std::string("test");
				case 1:
					return std::string("snow");
				default:
					return std::string("");
			}
		}
	};

	class ColorListModel : public gcn::ListModel
	{
	public:
		int getNumberOfElements()
		{
			return 3;
		}

		std::string getElementAt(int i)
		{
			switch(i)
			{
				case 0:
					return std::string("Red");
				case 1:
					return std::string("Black");
				case 2:
					return std::string("Yellow");
				default:
					return std::string("");
			}
		}
		
		Color getColorAt(int i)
		{
			switch (i)
			{
				case 0:
					return Color::RED;
				case 1:
					return Color::BLACK;
				case 2:
					return Color::YELLOW;
				default:
					return Color(0.5f, 0.5f, 0.5f);
			}
		}
		
	};

	class NumberOfPlayersListModel : public gcn::ListModel
	{
	public:
		int getNumberOfElements()
		{
			return 8;
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
				case 4:
					return std::string("5");
				case 5:
					return std::string("6");
				case 6:
					return std::string("7");
				case 7:
					return std::string("8");
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
		gcn::Gui* gui;
		gcn::ImageFont* font;
		gcn::ImageFont* bigFont;

		/*
		 * All of the default widgets
		 */
		gcn::Container* top;

		gcn::Label* labelHeader;

		gcn::Label* labelNumberOfPlayers;
		gcn::Label* labelMap;
		gcn::Label* labelHost;
		gcn::Label* labelColor;
		
		gcn::Button* buttonStartServer;
		gcn::Button* buttonConnectToServer;
		
		gcn::TextField* textFieldHost;

		gcn::DropDown* dropDownNumberOfPlayers;
		gcn::DropDown* dropDownMaps;
		gcn::DropDown* dropDownColors;
		
		gcn::CheckBox* checkBox1;
		gcn::Window *window;
		
		NumberOfPlayersListModel numberOfPlayersListModel;
		MapListModel mapListModel;
		ColorListModel colorListModel;
		Game *game;

	};
};

#endif

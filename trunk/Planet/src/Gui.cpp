#include "Gui.h"
#include "Game.h"

namespace Planet
{

	Gui::Gui()
	{
	}

	Gui::~Gui()
	{
		// Guichan stuff
		delete font;
		delete gui;

		// Widgets
		delete top;
		delete window;

		delete labelHeader;
		delete labelNumberOfPlayers;
		delete labelMap;
		delete labelHost;
		delete labelColor;

		delete buttonStartServer;
		delete buttonConnectToServer;
		
		delete textFieldHost;
		delete dropDownNumberOfPlayers;
		delete dropDownMaps;
		delete dropDownColors;

		delete checkBox1;


		// Guichan/SDL
		delete input;
		delete graphics;
		delete imageLoader;
	}

	void Gui::init(Game *game)
	{
		this->game = game;
	
		imageLoader = new gcn::OpenGLSDLImageLoader();

		gcn::Image::setImageLoader(imageLoader);
		graphics = new gcn::OpenGLGraphics();
		
		graphics->setTargetPlane(Game::WINDOW_SIZE.x, Game::WINDOW_SIZE.y);
		input = new gcn::SDLInput();

		top = new gcn::Container();
		top->setDimension(gcn::Rectangle(Game::WINDOW_SIZE.x/4, Game::WINDOW_SIZE.y/4, Game::WINDOW_SIZE.x/2, Game::WINDOW_SIZE.y/2));

		gui = new gcn::Gui();
		gui->setGraphics(graphics);
		gui->setInput(input);
		gui->setTop(top);

		//font = new gcn::ImageFont("images/fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		font = new gcn::ImageFont("images/rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""); 	
		bigFont = new gcn::ImageFont("images/techyfontbig.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""); 	
		gcn::Widget::setGlobalFont(font);

		initWidgets();
	}

	void Gui::initWidgets()
	{
		gcn::Color widgetColor(80, 80, 80, 100);
		gcn::Color widgetColor2(80, 80, 80, 200);
	
		labelHeader = new gcn::Label("PLaneT");
		labelHeader->setFont(bigFont);
		labelHeader->setHeight(20);
		labelHeader->setWidth(150);
			
		labelNumberOfPlayers = new gcn::Label("Number of players");
		labelMap = new gcn::Label("Map");
		labelHost = new gcn::Label("Host");
		labelColor = new gcn::Label("Color");

		buttonStartServer = new gcn::Button("Start server");
		buttonStartServer->setActionEventId("START_SERVER");
		buttonStartServer->addActionListener(this);
		buttonStartServer->setBaseColor(widgetColor);

		buttonConnectToServer = new gcn::Button("Connect to server");
		buttonConnectToServer->setActionEventId("CONNECT_TO_SERVER");
		buttonConnectToServer->addActionListener(this);
		buttonConnectToServer->setBaseColor(widgetColor);

		textFieldHost = new gcn::TextField("localhost");
		textFieldHost->setWidth(100);

		dropDownNumberOfPlayers = new gcn::DropDown(&numberOfPlayersListModel);
		dropDownNumberOfPlayers->setBaseColor(widgetColor2);
		
		dropDownMaps = new gcn::DropDown(&mapListModel);
		dropDownMaps->setBaseColor(widgetColor2);

		dropDownColors = new gcn::DropDown(&colorListModel);
		dropDownColors->setBaseColor(widgetColor2);

		checkBox1 = new gcn::CheckBox("Checkbox 1");
		window = new gcn::Window("Window");
		window->setBaseColor(gcn::Color(255, 150, 200, 190));
		window->resizeToContent();

		top->setBaseColor(widgetColor);

		// Add widgets to container
		top->add(labelHeader, 10, 10);
	
		top->add(labelNumberOfPlayers, 10, 60);
		top->add(labelMap, 10, 85);
		top->add(dropDownNumberOfPlayers, 200, 60);
		top->add(dropDownMaps, 200, 85);
		top->add(buttonStartServer, 200, 110);

		top->add(labelHost, 10, 150);
		top->add(textFieldHost, 200, 150);
		top->add(labelColor, 10, 175);
		top->add(dropDownColors, 200, 175);
		top->add(buttonConnectToServer, 200, 200);

		//top->add(checkBox1, 500, 130);
		//top->add(window, 100, 350);
	}
	
	void Gui::logic()
	{
		gui->logic();
	}
	
	void Gui::render()
	{
		gui->draw();
	}
	
	void Gui::pushInput(SDL_Event &event)
	{
		input->pushInput(event);
	}

	void Gui::action(const gcn::ActionEvent& actionEvent)
	{
		std::string host = textFieldHost->getText();
		int numberOfPlayers = dropDownNumberOfPlayers->getSelected() + 1;
		std::string map = mapListModel.getElementAt(dropDownMaps->getSelected());
		Color color = colorListModel.getColorAt(dropDownColors->getSelected());

		if (actionEvent.getId() == "START_SERVER")
		{
			game->startServer(numberOfPlayers, map);
			buttonStartServer->setEnabled(false);
			buttonStartServer->setCaption("Waiting");
		}
		
		if (actionEvent.getId() == "CONNECT_TO_SERVER")
		{
			bool connected = game->connectToServer(host, color);
			
			if (connected)
				game->toggleMenu();
		}
	}	
};

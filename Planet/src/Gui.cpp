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

		delete labelNumberOfPlayers;
		delete labelHost;

		delete buttonStartServer;
		delete buttonConnectToServer;
		
		delete textFieldHost;
		delete dropDownNumberOfPlayers;

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

		font = new gcn::ImageFont("images/fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		gcn::Widget::setGlobalFont(font);

		initWidgets();
	}

	void Gui::initWidgets()
	{
		labelNumberOfPlayers = new gcn::Label("Number of players");
		labelHost = new gcn::Label("Host");

		buttonStartServer = new gcn::Button("Start server");
		buttonStartServer->setActionEventId("START_SERVER");
		buttonStartServer->addActionListener(this);

		buttonConnectToServer = new gcn::Button("Connect to server");
		buttonConnectToServer->setActionEventId("CONNECT_TO_SERVER");
		buttonConnectToServer->addActionListener(this);

		textFieldHost = new gcn::TextField("localhost");
		textFieldHost->setWidth(100);

		dropDownNumberOfPlayers = new gcn::DropDown(&numberOfPlayersListModel);

		checkBox1 = new gcn::CheckBox("Checkbox 1");
		window = new gcn::Window("I am a window  Drag me");
		window->setBaseColor(gcn::Color(255, 150, 200, 190));
		window->resizeToContent();


		// Add widgets to container
		top->add(labelNumberOfPlayers, 10, 10);
		top->add(dropDownNumberOfPlayers, 150, 10);
		top->add(buttonStartServer, 150, 35);

		top->add(labelHost, 10, 80);
		top->add(textFieldHost, 150, 80);
		top->add(buttonConnectToServer, 150, 105);

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
	
		if (actionEvent.getId() == "START_SERVER")
		{
			game->startServer(numberOfPlayers);
			buttonStartServer->setEnabled(false);
			buttonStartServer->setCaption("Waiting");
		}
		
		if (actionEvent.getId() == "CONNECT_TO_SERVER")
		{
			bool connected = game->connectToServer(host);
			
			if (connected)
				game->toggleMenu();
		}
	}	
};

#include "Game.h"
#include "ConfigHandler.h"

int main(int argc, char **argv)
{
	std::string host;

	if (argc == 2)
	{
		host = argv[1];
	}
	else
	{
		// use default host
		host = "localhost";
	}

	Planet::Game game;
	game.run();
	
	return 0;
}


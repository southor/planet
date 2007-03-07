#include "Game.h"

bool running;

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
	//game.run2();
	
	return 0;
}


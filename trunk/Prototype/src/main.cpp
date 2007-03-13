#include "Game.h"
#include "KeyHandler.h"

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

	printf("Using remote host: %s\n", host.c_str());

	Prototype::Game game;
	game.run(host);
	
	return 0;
}


#include "Game.h"

bool running;

int main(int argc, char **argv)
{
	Prototype::Game game;
	game.run();
	//Game *game = new Game();
	//game->run();
	//delete game;

	return 0;
}

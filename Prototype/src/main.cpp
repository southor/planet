#include "game.h"
#include "KeyHandler.h"

bool running;

int main(int argc, char **argv)
{
	try
	{
		Prototype::Game game;
		game.run();
		//Game *game = new Game();
		//game->run();
		//delete game;
	}
	/*
	 * Catch all Guichan exceptions
	 */
	catch (gcn::Exception e)
	{
		std::cerr << e.getMessage() << std::endl;
		return 1;
	}
	/*
	 * Catch all Std exceptions
	 */
	catch (std::exception e)
	{
		std::cerr << "Std exception: " << e.what() << std::endl;
		return 1;
	}
	/*
	 * Catch all Unknown exceptions
	 */
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}

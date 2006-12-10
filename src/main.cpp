#include "planet.h"

bool running;

int main(int argc, char **argv)
{
	Planet *planet = new Planet();
	planet->run();
	delete planet;

	return 0;
}

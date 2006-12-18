#ifndef __player_h__
#define __player_h__

#include "color.h"

namespace Prototype
{
	class Player
	{
	public:
		Color color;
		
		Player(const Color &color) : color(color)	{}
	};

	typedef std::vector<Player> Players;
};

#endif
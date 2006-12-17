#ifndef __player_h__
#define __player_h__

#include "color.h"

namespace Prototype
{
	class Player
	{
	public:
		Color color;
	};

	typedef std::vector<Player> Players;
};

#endif
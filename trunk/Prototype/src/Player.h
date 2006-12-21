#ifndef __player_h__
#define __player_h__

#include "color.h"
#include "IdMap.h"

namespace Prototype
{
	class Player
	{
	public:
		Color color;
		size_t playerObjId;
		
		Player()											{}
		Player(const Color &color) : color(color)			{}
	};

	typedef IdMap<size_t, Player> Players;
};

#endif
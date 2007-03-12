#ifndef __player_h__
#define __player_h__

#include "Color.h"
#include "IdMap.h"
#include "basic.h"

namespace Prototype
{
	class Player
	{
	public:
		Color color;
		//size_t playerObjId;
		
		Player()											{}
		Player(const Color &color) : color(color)			{}
	};

	typedef IdMap<PlayerId, Player*> Players;
};

#endif

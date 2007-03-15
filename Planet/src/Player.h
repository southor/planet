#ifndef __Player_h__
#define __Player_h__

#include "Color.h"
#include "IdMap.h"
#include "basic.h"

namespace Planet
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

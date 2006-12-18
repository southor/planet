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

	// Functor
	template <class T>
	class HasPlayerId
	{
	private:
		int playerId;
	public:
		HasPlayerId(int playerId) : playerId(playerId)	{}
		bool operator ()(const T &o)	{ return o.getPlayerId() == this->playerId; }
		bool operator ()(const T *o)	{ return o->getPlayerId() == this->playerId; }
	};

	typedef std::vector<Player> Players;
};

#endif
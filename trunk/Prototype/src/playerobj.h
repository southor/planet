#ifndef __playerobj_h__
#define __playerobj_h__

#include "movableobj.h"
#include "rectangle.h"

namespace Prototype
{
	class PlayerObj : public MovableObj
	{
	private:
		int playerId;
	public:

		static const float RECTANGLE_SIZE;
		Pos pos;
		float angle;

		PlayerObj(int playerId, const Pos &pos) : playerId(playerId), pos(pos), angle(0.0f)	{}

		Pos getPos() const			{ return pos; }
		void getRectangle(Rectangle &rectangle) const;
		int getPlayerId() const		{ return playerId; }
	};
};

#endif
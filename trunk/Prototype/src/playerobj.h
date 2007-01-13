#ifndef __playerobj_h__
#define __playerobj_h__

#include "movableobj.h"
#include "rectangle.h"

namespace Prototype
{
	class PlayerObj : public MovableObj
	{
	private:
		//size_t playerId;
	public:

		static const float RECTANGLE_SIZE;
		Pos pos;
		float angle;

		bool movingForward;
		bool movingBackward;
		bool strafingLeft;
		bool strafingRight;

		//PlayerObj(size_t playerId, const Pos &pos);
		PlayerObj(const Pos &pos);

		Pos getPos() const			{ return pos; }
		void getRectangle(Rectangle &rectangle) const;
		//size_t getPlayerId() const		{ return playerId; }
	};
};

#endif
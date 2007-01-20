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

		static const float FORWARD_BACKWARD_SPEED;
		static const float STRAFE_SPEED;
		static const float RECTANGLE_SIZE;
		Pos pos;
		float angle;

		int health;

		bool movingForward;
		bool movingBackward;
		bool strafingLeft;
		bool strafingRight;

		//PlayerObj(size_t playerId, const Pos &pos);
		PlayerObj(const Pos &pos);

		Pos getPos() const			{ return pos; }
		void getRectangle(Rectangle &rectangle) const;
		//size_t getPlayerId() const		{ return playerId; }	

		void hurt(int damage);

		bool isDead()			{ return health <= 0; }

		void respawn(const Pos &respawnPos);
	};
};

#endif
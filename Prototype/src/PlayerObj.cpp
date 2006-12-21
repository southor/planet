
#include "PlayerObj.h"

namespace Prototype
{

	const float PlayerObj::RECTANGLE_SIZE = 64.0f;

	PlayerObj::PlayerObj(size_t playerId, const Pos &pos)
		: playerId(playerId), pos(pos), angle(0.0f),
		 movingForward(false), movingBackward(false),
		 strafingLeft(false), strafingRight(false)
	{}

	void PlayerObj::getRectangle(Rectangle &rectangle) const
	{
		Vec size(RECTANGLE_SIZE, RECTANGLE_SIZE);
		
		rectangle.pos = this->pos - size/2.0f;
		rectangle.size = size;
	}
}

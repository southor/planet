
#include "PlayerObj.h"

namespace Prototype
{

	const float PlayerObj::RECTANGLE_SIZE = 64.0f;

	void PlayerObj::getRectangle(Rectangle &rectangle) const
	{
		Vec size(RECTANGLE_SIZE, RECTANGLE_SIZE);
		
		rectangle.pos = this->pos - size/2.0f;
		rectangle.size = size;
	}
}

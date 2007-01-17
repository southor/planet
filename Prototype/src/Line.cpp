#include "line.h"

namespace Prototype
{
	float Line::crossPoint(const Line &line)
	{
		float k = line.ext.y / line.ext.x;

		float d = (k*(this->pos.x - line.pos.x) + line.pos.y - this->pos.y) /
					(this->ext.y - k * this->ext.x);

		return d;
	}
};

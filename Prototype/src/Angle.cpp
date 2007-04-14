#include "Angle.h"

namespace Prototype
{
	const Angle Angle::PI = Angle(PI_F);

	float Angle::unifyAngle(float angle)
	{
		if ((angle > TWO_PI_F) || (angle < 0.0f))
		{				
			float div = angle / TWO_PI_F;
			angle = (div - static_cast<int>(div)) * TWO_PI_F;
		}
		return angle;
	}

	Angle Angle::operator -(Angle rh) const
	{
		float diff = angle - rh.angle;
		if (diff > PI_F) diff -= TWO_PI_F;
		else if (diff < -PI_F) diff += TWO_PI_F;
		return Angle(diff);
	}

};



#ifndef __angle_h__
#define __angle_h__

#include "basic.h"

namespace Prototype
{
	class Angle
	{
	private:
		
		float angle;
		float unifyAngle(float angle);

	public:

		static const Angle PI;

		inline Angle()
		{}
		
		inline Angle(float angle)
		{
			this->angle = unifyAngle(angle);
		}

		inline Angle operator +(Angle rh) const		{ return Angle(angle + rh.angle); }

		Angle operator -(Angle rh) const;

		inline Angle operator *(float t) const		{ return Angle(angle * t); }

		inline Angle operator /(float t) const		{ return Angle(angle / t); }

		inline const Angle& operator +=(Angle rh)
		{
			angle = unifyAngle(angle + rh.angle);
			return *this;
		}

		inline const Angle& operator -=(Angle rh)
		{
			angle = unifyAngle((*this - rh).angle);
			return *this;
		}

		inline const bool operator ==(Angle rh) const
		{			
			return angle == rh.angle;
		}

		inline const bool operator !=(Angle rh) const
		{			
			return angle != rh.angle;
		}

		//inline operator float ()
		//{
		//	return angle;
		//}

		inline float getFloat() const			{ return angle; }

	};

	//inline float sin(Angle angle)				{ return sin(angle.getFloat()); }
	//inline float cos(Angle angle)				{ return cos(angle.getFloat()); }
	

};

#endif

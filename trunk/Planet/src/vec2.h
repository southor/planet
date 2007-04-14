#ifndef __vec2_h__
#define __vec2_h__

#include <cmath>

namespace Planet
{

	template <typename T>
	class Vec2
	{
	public:
		T x;
		T y;

		Vec2()											{}

		Vec2(T x, T y) : x(x), y(y)						{}

		inline Vec2<T> operator+(T scalar) const
		{
			return Vec2(x + scalar, y + scalar);
		}

		inline Vec2<T> operator-(T scalar) const
		{
			return Vec2(x - scalar, y - scalar);
		}

		inline Vec2<T> operator*(T scalar) const
		{
			return Vec2(x * scalar, y * scalar);
		}

		inline Vec2<T> operator/(T scalar) const
		{
			return Vec2(x / scalar, y / scalar);
		}

		friend inline Vec2<T> operator+(T scalar, const Vec2<T>& rh)
		{
			return rh+scalar;
		}

		friend inline Vec2<T> operator*(T scalar, const Vec2<T>& rh)
		{
			return rh*scalar;
		}

		
		inline Vec2<T> operator+(const Vec2<T>& rh) const
		{
			return Vec2<T>(x + rh.x, y + rh.y);
		}

		inline Vec2<T> operator-(const Vec2<T>& rh) const
		{
			return Vec2<T>(x - rh.x, y - rh.y);
		}

		inline Vec2<T> operator*(const Vec2<T>& rh) const
		{
			return Vec2<T>(x * rh.x, y * rh.y);
		}

		inline Vec2<T> operator/(const Vec2<T>& rh) const
		{
			return Vec2<T>(x / rh.x, y / rh.y);						
		}

		inline const Vec2<T>& operator +=(const Vec2<T>& rh)
		{
			x += rh.x;
			y += rh.y;
			return *(this);
		}

		inline const Vec2<T>& operator -=(const Vec2<T>& rh)
		{
			x -= rh.x;
			y -= rh.y;
			return *(this);
		}

		inline const Vec2<T>& operator *=(T scalar)
		{
			x *= scalar;
			y *= scalar;
			return *(this);
		}

		inline const Vec2<T>& operator *=(const Vec2<T>& rh)
		{
			x *= rh.x;
			y *= rh.y;
			return *(this);
		}

		inline const Vec2<T>& operator /=(const Vec2<T>& rh)
		{
			x /= rh.x;
			y /= rh.y;
			return *(this);
		}

		inline Vec2<T> operator-() const
		{
			return Vec2(-x, -y);
		}

		inline bool operator==(const Vec2<T>& rh) const
		{
			return (rh.x == x) && (rh.y == y);
		}

		inline bool operator!=(const Vec2<T>& rh) const
		{
			return (rh.x != x) || (rh.y != y);
		}



		inline T length() const
		{
			return sqrt(x*x + y*y);
		}

		inline T length2() const
		{
			return x*x + y*y;
		}

		void normalize()
		{
			float tmp = 1.0f / length();
			x = x * tmp;
			y = y * tmp;
		}

	};


};

#endif


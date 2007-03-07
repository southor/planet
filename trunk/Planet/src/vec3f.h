#ifndef __vec3f_h__
#define __vec3f_h__

#include <cmath>
#include "SpherePoint.h"
#include "mat3f.h"

namespace Planet
{

	class Vec3f
	{
	public:
		float x;
		float y;
		float z;

		Vec3f() 
		{
			x = y = z = 0.0f;
		}

		Vec3f(float x, float y, float z) : x(x), y(y), z(z)	{}

		float operator|(const Vec3f& rh) const
		{
			return x * rh.x + y * rh.y + z * rh.z;
		}

		Vec3f crossproduct(const Vec3f& v) const
		{
			return Vec3f(y*v.z - z*v.y,
						z*v.x - x*v.z,
						x*v.y - y*v.x);
		}

		Vec3f operator*(float scalar) const
		{
			return Vec3f(x * scalar,
						y * scalar,
						z * scalar);
		}

		Vec3f operator/(float scalar) const
		{
			return Vec3f(x / scalar,
						y / scalar,
						z / scalar);
		}


		friend Vec3f operator*(float scalar, const Vec3f& rh)
		{
			return rh*scalar;
		}

		Vec3f operator-(const Vec3f& rh) const
		{
			return Vec3f(x - rh.x,
						y - rh.y,
						z - rh.z);
		}

		Vec3f operator+(const Vec3f& rh) const
		{
			return Vec3f(x + rh.x,
						y + rh.y,
						z + rh.z);
		}

		Vec3f operator/(const Vec3f& rh) const
		{
			return Vec3f(x / rh.x,
						y / rh.y,
						z / rh.z);
		}

		const Vec3f& operator +=(const Vec3f& rh)
		{
			x += rh.x;
			y += rh.y;
			z += rh.z;
			return *(this);
		}

		const Vec3f& operator -=(const Vec3f& rh)
		{
			x -= rh.x;
			y -= rh.y;
			z -= rh.z;
			return *(this);
		}

		const Vec3f& operator *=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *(this);
		}

		Vec3f operator-() const
		{
			return Vec3f(-x, -y, -z);
		}

		float length() const
		{
			return std::sqrt(x*x + y*y + z*z);
		}

		float length2() const
		{
			return x*x + y*y + z*z;
		}

		void normalize()
		{
			float tmp = 1.0f / length();
			x = x * tmp;
			y = y * tmp;
			z = z * tmp;
		}
		
		SpherePoint toSpherePoint()
		{
			SpherePoint sp;
			CartesianToSpherical(x, y, z, &(sp.p), &(sp.phi), &(sp.theta));
			return sp;
		}
		
/*
		static void transformVec(GLfloat *matrix, GLfloat *from, GLfloat *to) 
		{
			for(int i=0;i<4.0f;i++) 
			{ 
				*(to+i) = 0.0f; 
				for(int j=0;j<4;j++) 
				{ 
					*(to+i) = *(to+i) + (*(from+j) * (*(matrix+j*4+i))); 
				} 
			} 
		}
*/
	};


};

#endif


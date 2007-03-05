#include "mat3f.h"
#include "basic.h"

namespace Prototype
{
	Mat3f operator*(const Mat3f& m, const Mat3f& n)
	{
		Mat3f result;

		for (int j=0; j<3; ++j)
		{
			for (int i=0; i<3; ++i)
			{
				result(i, j) = 
					m(i, 0) * n(0, j) + 
					m(i, 1) * n(1, j) + 
					m(i, 2) * n(2, j);
			}
		}

		return result;
	}

	Vector3 operator*(const Mat3f& m, const Vector3& v)
	{
		return Vector3(
			m(0,0) * v.x + m(0,1) * v.y + m(0,2) * v.z,
			m(1,0) * v.x + m(1,1) * v.y + m(1,2) * v.z,
			m(2,0) * v.x + m(2,1) * v.y + m(2,2) * v.z);
	}

	void CartesianToSpherical(Vec3f &v, float *p, float *phi, float *theta)
	{
		float S;
		
		*p = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
		S = sqrt(v.x*v.x + v.y*v.y);

		*phi = acos(v.z / (*p));
		
		if (v.x >= 0)
			*theta = asin(v.y / S);
		else
			*theta = PI_F - asin(v.y / S);
	}
	
	Vec3f SphericalToCartesian(float p, float phi, float theta)
	{
		Vec3f v;
		
		v.x = p * sin(phi) * cos(theta);
		v.y = p * sin(phi) * sin(theta);
		v.z = p * cos(phi);

		return v;
	}

};
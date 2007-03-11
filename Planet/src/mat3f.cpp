#include "mat3f.h"
#include "basic.h"
#include "vec3f.h"

namespace Planet
{
/*
	static Mat3f rotateZ(real angle)
	{
		real sinAngle = sin(angle);
		real cosAngle = cos(angle);
		Mat3f matrix = Mat3f::identity();
		matrix.data[0] = cosAngle;
		matrix.data[1] = sinAngle;
		matrix.data[3] = -sinAngle;
		matrix.data[4] = cosAngle;
		return matrix;
	}
		*/
	Mat3f Mat3f::rotateArbitrary(Vector3 &vv, real angle)
	{
		Vec3f v = vv;
		v.normalize();
	
		real sinAngle = sin(angle);
		real cosAngle = cos(angle);
		Mat3f matrix = Mat3f::identity();

		matrix.data[0] = cosAngle + (1 - cosAngle) * v.x * v.x;
		matrix.data[1] = (1 - cosAngle) * v.y * v.x + sinAngle * v.z;
		matrix.data[2] = (1 - cosAngle) * v.z * v.x -  sinAngle * v.y;

		matrix.data[3] = (1 - cosAngle) * v.x * v.y - sinAngle * v.z;
		matrix.data[4] = cosAngle + (1 - cosAngle) * v.y * v.y;
		matrix.data[5] = (1 - cosAngle) * v.z * v.y + sinAngle * v.x;

		matrix.data[6] = (1 - cosAngle) * v.x * v.z + sinAngle * v.y;
		matrix.data[7] = (1 - cosAngle) * v.y * v.z - sinAngle * v.x;
		matrix.data[8] = cosAngle + (1 - cosAngle) * v.z * v.z;

		return matrix;
	}



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

	void CartesianToSpherical(float x, float y, float z, float *p, float *phi, float *theta)
	{
		float S;
		
		*p = sqrt(x*x + y*y + z*z);
		S = sqrt(x*x + y*y);

		*phi = acos(z / (*p));
		
		if (x == 0 && y == 0)
			*theta = 0.0f;
		else if (x >= 0)
			*theta = asin(y / S);
		else
			*theta = PI_F - asin(y / S);
			
	}
	
	void SphericalToCartesian(float p, float phi, float theta, float *x, float *y, float *z)
	{
		*x = p * sin(phi) * cos(theta);
		*y = p * sin(phi) * sin(theta);
		*z = p * cos(phi);
	}
};
#include "mat3f.h"

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
};
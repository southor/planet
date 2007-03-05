#ifndef __math3f_h__
#define __math3f_h__

#include <string.h>

#include "vec3f.h"

namespace Prototype
{	
	typedef Vec3f Vector3;
	typedef float real;

	class Mat3f
	{
	public:
		real data[9];

		Mat3f()	{}

		explicit Mat3f(const real* p)
		{
			for(int i=0; i<9; ++i)
				data[i] = p[i];			
		}

		Mat3f( const Mat3f &rh )
		{
			memcpy( this, &rh, sizeof(Mat3f) );
		}

		Mat3f &operator=( const Mat3f &rh )
		{
			memcpy( this, &rh, sizeof(Mat3f) );
			return *this;
		}

		real& operator()(int i, int j) 
		{
			return data[i+3*j]; 
		}

		real operator()(int i, int j) const 
		{ 
			return data[i+3*j];
		}

		static const Mat3f& identity()
		{
			static real data[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
			static Mat3f id(data);

			return id;
		}

		static Mat3f rotateX(real angle)
		{
			real sinAngle = sin(angle);
			real cosAngle = cos(angle);
			Mat3f matrix = Mat3f::identity();
			matrix.data[4]  = cosAngle;
			matrix.data[5]  = sinAngle;
			matrix.data[7]  = -sinAngle;
			matrix.data[8] = cosAngle;
			return matrix;
		}

		static Mat3f rotateY(real angle)
		{
			real sinAngle = sin(angle);
			real cosAngle = cos(angle);
			Mat3f matrix = Mat3f::identity();
			matrix.data[0]  = cosAngle;
			matrix.data[2]  = -sinAngle;
			matrix.data[6]  = sinAngle;
			matrix.data[8] = cosAngle;
			return matrix;
		}

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
	};
	
	
	Mat3f operator*(const Mat3f& m, const Mat3f& n);

	Vector3 operator*(const Mat3f& m, const Vector3& v);
	
	void CartesianToSpherical(Vec3f &v, float *p, float *phi, float *theta);
	
	Vec3f SphericalToCartesian(float p, float phi, float theta);

	
		
};

#endif
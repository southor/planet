#include "SpherePoint.h"

#include "vec3f.h"

namespace Planet
{
	SpherePoint::SpherePoint(Vec3f &v)
	{
		CartesianToSpherical(v.x, v.y, v.z, &p, &phi, &theta);
	}

	Vec3f SpherePoint::toVector()
	{ 
		Vec3f v;
		SphericalToCartesian(p, phi, theta, &(v.x), &(v.y), &(v.z)); 
		return v;
	}
};
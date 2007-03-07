#ifndef __SpherePoint_h__
#define __SpherePoint_h__

namespace Planet
{
	class Vec3f;

	class SpherePoint
	{
	public:
		SpherePoint() : p(0.0f), phi(0.0f), theta(0.0f)	{}
		SpherePoint(float p, float phi, float theta) : p(p), phi(phi), theta(theta) {}
		SpherePoint(Vec3f &v);
	
		Vec3f toVector();
		
		float p;
		float phi;
		float theta;
	};
};
#endif

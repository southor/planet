#ifndef __planet_h__
#define __planet_h__

#include "common.h"
#include "basic.h"

namespace Prototype
{
	class Planet
	{
	public:
		Planet() : viewAngle(0.0f) {}
	
		void render();
		
	
		float viewAngle;
		float viewAngle2;

	private:

	};
};

#endif

#ifndef __SkyBox_h__
#define __SkyBox_h__

#include "common.h"
#include "basic.h"

namespace Planet
{
	class SkyBox
	{
	public:
		SkyBox() {}
		
		void init();		
		void render(Vec3f &position);
	
	private:
		uint textures[6];
	};
};

#endif
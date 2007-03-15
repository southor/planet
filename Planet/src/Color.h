#ifndef __Color_h__
#define __Color_h__

namespace Planet
{
	struct Color
	{
		float r;
		float g;
		float b;

		Color()													{}
		Color(float r, float g, float b) : r(r), g(g), b(b)		{}

		static const Color BLACK;
		static const Color RED;
		static const Color YELLOW;
	};
};

#endif



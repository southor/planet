#ifndef __color_h__
#define __color_h__

namespace Prototype
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


#ifndef __postranslate_h__
#define __postranslate_h__

#include "Rectangle.h"

namespace Prototype
{

	/**
	 * Translates from screen coordinates to Game position.
	 */
	class PosTranslate
	{
	public:

		// Render area in game coordinates
		Rectangle renderArea;
		
		// Render area in screen coordinates
		Vec2<Uint16> screenRenderPos;
		Vec2<Uint16> screenRenderSize;

		virtual ~ClientWorldModel();

		/**
		 * @param screenPos position in screen coordinates
		 * @return position in game coordinates
		 */
		Pos operator ()(Vec2<Uint16> screenPos)
		{
			Vec screenRenderSizef(static_cast<float>(screenRenderSize.x), static_cast<float>(screenRenderSize.y));

			screenPos -= screenRenderPos;
			
			Pos pos(static_cast<float>(screenPos.x), static_cast<float>(screenPos.y));
			pos /= screenRenderSize;
			pos *= renderArea.size;
			pos += renderArea.pos;
			
			return pos;			
		}

	};
};

#endif
#ifndef __viewporthandler_h__
#define __viewporthandler_h__

#include "Rectangle.h"

namespace Prototype
{

	/**
	 * Translates from screen coordinates to Game position.
	 */
	class ViewportHandler
	{
	public:
		
		// Render area in GL screen coordinates
		Vec2<int> screenRenderPos;
		Vec2<int> screenRenderSize;

		virtual ~ViewportHandler()		{}

		//Setups gl viewport
		void useViewport();

		/**
		 * @param screenPos position in GL screen coordinates
		 * @return position in game coordinates
		 */
		virtual Pos screenToGame(const Rectangle &renderArea, Vec2<int> screenPos);

	};
};

#endif

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

		// Render area in game coordinates
		Rectangle renderArea;
		
		// Render area in screen coordinates
		Vec2<int> screenRenderPos;
		Vec2<int> screenRenderSize;

		virtual ~ViewportHandler()		{}

		//Setups gl viewport
		void useViewport();

		/**
		 * @param screenPos position in screen coordinates
		 * @return position in game coordinates
		 */
		virtual Pos screenToGame(Vec2<int> screenPos);

	};
};

#endif
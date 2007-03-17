#ifndef __viewporthandler_h__
#define __viewporthandler_h__

#include "basic.h"

namespace Planet
{

	/**
	 * Translates from screen coordinates to Game position.
	 */
	class ViewportHandler
	{
	public:

		// Render area in game coordinates
		//Rectangle renderArea;
		
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
		//virtual Pos screenToGame(const Rectangle &renderArea, Vec2<int> screenPos);

	};
};

#endif

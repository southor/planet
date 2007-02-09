#include "ViewportHandler.h"
#include "common.h"

namespace Prototype
{

	void ViewportHandler::useViewport()
	{
		glViewport(screenRenderPos.x, screenRenderPos.y, screenRenderSize.x, screenRenderSize.y);
	}

	Pos ViewportHandler::screenToGame(Vec2<int> screenPos)
	{
		
		Vec screenRenderSizef(static_cast<float>(screenRenderSize.x), static_cast<float>(screenRenderSize.y));

		screenPos -= screenRenderPos;
		
		Pos pos(static_cast<float>(screenPos.x), static_cast<float>(screenPos.y));
		pos /= screenRenderSizef;
		pos *= renderArea.size;
		pos += renderArea.pos;
		
		return pos;			
	}

};


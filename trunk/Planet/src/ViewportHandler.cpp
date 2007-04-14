#include "ViewportHandler.h"
#include "common.h"

namespace Planet
{

	void ViewportHandler::useViewport()
	{
		glViewport(screenRenderPos.x, screenRenderPos.y, screenRenderSize.x, screenRenderSize.y);
	}

};


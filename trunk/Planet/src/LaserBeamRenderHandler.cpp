#ifndef _LaserBeamRenderHandler_cpp_
#define _LaserBeamRenderHandler_cpp_

#include "LaserBeamRenderHandler.h"
#include "common.h"

#include "externdata.inl"


namespace Planet
{
	LaserBeamRenderHandler::LaserBeamRenderHandler(size_t initCapacity, float growFactor) : externLB(initCapacity, growFactor)
	{
	}

	LaserBeamRenderHandler::~LaserBeamRenderHandler()
	{
	}

	void LaserBeamRenderHandler::setupRender()
	{
		glEnable(GL_BLEND);		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		//glEnableClientState(GL_VERTEX_ARRAY);
	}
	
	void LaserBeamRenderHandler::render()
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		
		glVertexPointer(4, GL_FLOAT, 0, getVtxArr());
		glColorPointer(4, GL_FLOAT, 0, getColorArr());
		
		glDrawArrays(GL_QUADS, 0, static_cast<uint>(getSize()));
		
		glDisableClientState(GL_COLOR_ARRAY);
	}
};

#endif



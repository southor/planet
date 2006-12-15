#ifndef __worldrenderer_cpp__
#define __worldrenderer_cpp__


#include "worldrenderer.h"
#include <algorithm>

namespace Prototype
{

	// --------------------------------------------------------------------------------------
	// ----------------------------------   WorldRenderer  ----------------------------------
	// --------------------------------------------------------------------------------------


	WorldRenderer::WorldRenderer(RenderMode renderMode) : renderMode(renderMode)
	{}

	void WorldRenderer::render(WorldModel &worldModel, PlayerObj *currentPlayer)
	{
		// execute camera properties
		switch(renderMode)
		{
		case HOLE_WORLD:
			//TODO some gl calls here
			break;
		default:
			assert(false);
			break;
		}

		// render all objects
		RenderGameObj renderGameObj(currentPlayer);
		std::for_each(worldModel.getObstacles().begin(), worldModel.getObstacles().end(), renderGameObj);
		std::for_each(worldModel.getPlayerObjs().begin(), worldModel.getPlayerObjs().end(), renderGameObj);
		std::for_each(worldModel.getProjectiles().begin(), worldModel.getProjectiles().end(), renderGameObj);
	}


	// --------------------------------------------------------------------------------------
	// -------------------------------   RenderGameObject  ----------------------------------
	// --------------------------------------------------------------------------------------

	void WorldRenderer::RenderGameObj::operator ()(const Obstacle* obstacle)
	{
		//TODO
	}

	void WorldRenderer::RenderGameObj::operator ()(const PlayerObj* playerObj)
	{
		//TODO
	}

	void WorldRenderer::RenderGameObj::operator ()(const Projectile* projectile)
	{
		//TODO
	}

};

#endif
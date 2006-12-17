#ifndef __worldrenderer_cpp__
#define __worldrenderer_cpp__


#include "worldrenderer.h"
#include "common.h"
#include <algorithm>

namespace Prototype
{

	// --------------------------------------------------------------------------------------
	// ----------------------------------   WorldRenderer  ----------------------------------
	// --------------------------------------------------------------------------------------


	WorldRenderer::WorldRenderer(RenderMode renderMode)
		: renderMode(renderMode)
	{}

	void WorldRenderer::setupProjection()
	{
		glMatrixMode(GL_PROJECTION);
		glOrtho(0.0, WorldModel::WORLD_SIZE.x, 0.0, WorldModel::WORLD_SIZE.y, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void WorldRenderer::render(WorldModel &worldModel, Players &players, PlayerObj *localPlayerObj)
	{
		glLoadIdentity();
		
		// execute camera properties
		switch(renderMode)
		{
		case HOLE_WORLD:
			// nothing to do, we are rendering the hole world
			break;
		default:
			assert(false);
			break;
		}

		// render all objects
		RenderGameObj renderGameObj(&players);
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
		
		Rectangle rect;
		playerObj->getRectangle(rect);
		
		// setup some vertexes
		Vec2f v0(rect.getTopRight());
		Vec2f v1(rect.getTopLeft());
		Vec2f v2(rect.getBottomLeft());
		Vec2f v3(rect.getBottomRight());
		Vec2f vp((v0+v1) / 2.0f);

		// get a player color
		int playerId = playerObj->getPlayerId();
		Color playerColor(0.7f, 0.7f, 0.7f);
		if ((static_cast<int>(players->size()) > playerId) && (playerId >= 0))
		{
			playerColor = (*players)[playerId].color;
		}

		// Render rectangle
		glBegin(GL_QUADS);
			glColor3f(0.0f,0.0f,0.0f);
			glVertex2fv(reinterpret_cast<float*>(&v0));
			glVertex2fv(reinterpret_cast<float*>(&v1));
			glVertex2fv(reinterpret_cast<float*>(&v2));
			glVertex2fv(reinterpret_cast<float*>(&v3));
		glEnd();

		// Render triangle	
		glBegin(GL_TRIANGLES);
			glColor3fv(reinterpret_cast<float*>(&playerColor));
			glVertex2fv(reinterpret_cast<float*>(&vp));
			glColor3f(0.0f,0.0f,0.0f);
			glVertex2fv(reinterpret_cast<float*>(&v2));
			glVertex2fv(reinterpret_cast<float*>(&v3));
		glEnd();
	}

	void WorldRenderer::RenderGameObj::operator ()(const Projectile* projectile)
	{
		//TODO
	}

};

#endif
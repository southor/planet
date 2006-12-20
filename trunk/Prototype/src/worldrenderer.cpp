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
		glLoadIdentity();
		glOrtho(0.0, WorldModel::WORLD_SIZE.x, 0.0, WorldModel::WORLD_SIZE.y, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
	}

	void WorldRenderer::render(WorldModel &worldModel, Players &players, PlayerObj *localPlayerObj)
	{
		glLoadIdentity();

		// render box around render area
		renderViewBox();
		
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
		ForEach(worldModel.getObstacles().begin(), worldModel.getObstacles().end(), renderGameObj);
		ForEach(worldModel.getPlayerObjs().begin(), worldModel.getPlayerObjs().end(), renderGameObj);
		ForEach(worldModel.getProjectiles().begin(), worldModel.getProjectiles().end(), renderGameObj);
		//std::for_each(worldModel.getObstacles().begin(), worldModel.getObstacles().end(), renderGameObj);
		//std::for_each(worldModel.getPlayerObjs().begin(), worldModel.getPlayerObjs().end(), renderGameObj);
		//std::for_each(worldModel.getProjectiles().begin(), worldModel.getProjectiles().end(), renderGameObj);
	}

	void WorldRenderer::renderRectangle(const Rectangle &rect, GLenum mode)
	{
		// setup some vertexes
		Vec2f v0(rect.getTopRight());
		Vec2f v1(rect.getTopLeft());
		Vec2f v2(rect.getBottomLeft());
		Vec2f v3(rect.getBottomRight());

		// Render rectangle
		glBegin(mode);
			glVertex2fv(reinterpret_cast<float*>(&v0));
			glVertex2fv(reinterpret_cast<float*>(&v1));
			glVertex2fv(reinterpret_cast<float*>(&v2));
			glVertex2fv(reinterpret_cast<float*>(&v3));
		glEnd();
	}

	void WorldRenderer::renderViewBox()
	{
		// render rectangle
		glColor3f(0.0f,0.0f,0.0f);
		renderRectangle(Rectangle(Pos(0.0f, 0.0f), WorldModel::WORLD_SIZE), GL_LINE_LOOP);
	}

	// --------------------------------------------------------------------------------------
	// -------------------------------   RenderGameObject  ----------------------------------
	// --------------------------------------------------------------------------------------

	void WorldRenderer::RenderGameObj::operator ()(const Obstacle* obstacle)
	{
		glColor3f(0.0f,0.0f,0.0f);
		WorldRenderer::renderRectangle(*obstacle, GL_QUADS);
	}

	void WorldRenderer::RenderGameObj::operator ()(const PlayerObj* playerObj)
	{
		Rectangle rect;
		playerObj->getRectangle(rect);

		// render rectangle
		glColor3f(0.0f,0.0f,0.0f);
		WorldRenderer::renderRectangle(rect, GL_QUADS);
		
		// setup some vertexes
		Vec2f v0((rect.getTopLeft() + rect.getTopRight()) / 2.0f);
		Vec2f v1(rect.getBottomLeft());
		Vec2f v2(rect.getBottomRight());

		// get a player color
		int playerId = playerObj->getPlayerId();
		Color playerColor(0.7f, 0.7f, 0.7f);
		if ((static_cast<int>(players->size()) > playerId) && (playerId >= 0))
		{
			playerColor = (*players)[playerId].color;
		}

		// render triangle
		glPushMatrix();
			glTranslatef(playerObj->pos.x, playerObj->pos.y, 0.0f);
			glRotatef(radianToDegree(playerObj->angle), 0.0f, 0.0f, 1.0f); 
			glTranslatef(-playerObj->pos.x, -playerObj->pos.y, 0.0f);
			glBegin(GL_TRIANGLES);
				glColor3fv(reinterpret_cast<float*>(&playerColor));
				glVertex2fv(reinterpret_cast<float*>(&v0));
				glColor3f(0.0f,0.0f,0.0f);
				glVertex2fv(reinterpret_cast<float*>(&v1));
				glVertex2fv(reinterpret_cast<float*>(&v2));
			glEnd();
		glPopMatrix();
	}

	void WorldRenderer::RenderGameObj::operator ()(const Projectile* projectile)
	{
		//TODO
	}

};

#endif
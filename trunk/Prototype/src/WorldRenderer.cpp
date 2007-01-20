#ifndef __worldrenderer_cpp__
#define __worldrenderer_cpp__


#include "worldrenderer.h"
#include "common.h"
#include <algorithm>

namespace Prototype
{

	const float WorldRenderer::Explosion::BULLET_EXPLOSION_SIZE = 10.0f;
	const float WorldRenderer::Explosion::ROCKET_EXPLOSION_SIZE = 25.0f;

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
		
		//render explosion
		ForEach(explosions.begin(), explosions.end(), renderGameObj);
		explosions.clear();
	}

	void WorldRenderer::projectileHit(Projectile *projectile, const Pos &hitPos)
	{
		float size;
		Color color = Color::BLACK;

		if (projectile->getType() == Projectile::BULLET) size = Explosion::BULLET_EXPLOSION_SIZE;
		else if (projectile->getType() == Projectile::ROCKET) size = Explosion::ROCKET_EXPLOSION_SIZE;
		
		Explosion explosion = {hitPos, size, color};
		explosions.push_back(explosion);
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

	void WorldRenderer::renderLine(const Line &line, float lineWidth, float alpha)
	{
		Pos startPos(line.pos);
		Pos endPos(line.getEndPos());

		glLineWidth(lineWidth);
		glPushMatrix();			
			glBegin(GL_LINES);
				glVertex2fv(reinterpret_cast<float*>(&startPos));				
				glVertex2fv(reinterpret_cast<float*>(&endPos));				
			glEnd();
		glPopMatrix();
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

	void WorldRenderer::RenderGameObj::operator ()(const WorldModel::ObstacleContainer::Pair &obstaclePair)
	{
		glColor3f(0.0f,0.0f,0.0f);
		WorldRenderer::renderRectangle(*(obstaclePair.second), GL_QUADS);
	}

	void WorldRenderer::RenderGameObj::operator ()(const WorldModel::PlayerObjContainer::Pair &playerObjPair)
	{
		static const Color PLAYER_RECTANGLE_COLOR = Color(0.7f,0.7f,0.7f);
		static const float PLAYER_RECTANGLE_ALPHA = 0.5f;
		
		size_t playerId = playerObjPair.first;
		PlayerObj *playerObj = playerObjPair.second;
		
		
		Rectangle rect;
		playerObj->getRectangle(rect);

		// render rectangle
		glColor4f(PLAYER_RECTANGLE_COLOR.r, PLAYER_RECTANGLE_COLOR.g, PLAYER_RECTANGLE_COLOR.b, PLAYER_RECTANGLE_ALPHA);
		WorldRenderer::renderRectangle(rect, GL_QUADS);
		
		// setup some vertexes
		Vec2f v0((rect.getTopLeft() + rect.getTopRight()) / 2.0f);
		Vec2f v1(rect.getBottomLeft());
		Vec2f v2(rect.getBottomRight());

		// get a player color
		//size_t playerId = playerObj->getPlayerId();
		//Color playerColor(0.7f, 0.7f, 0.7f);
		//if (players->isValid(playerId))
		//{
		Color playerColor = (*players)[playerId].color;
		//}

		// render triangle
		glPushMatrix();
			glTranslatef(playerObj->pos.x, playerObj->pos.y, 0.0f);
			glRotatef(radianToDegree(playerObj->angle) - 90.0f, 0.0f, 0.0f, 1.0f); 
			glTranslatef(-playerObj->pos.x, -playerObj->pos.y, 0.0f);
			glBegin(GL_TRIANGLES);
				glColor3fv(reinterpret_cast<float*>(&playerColor));
				glVertex2fv(reinterpret_cast<float*>(&v0));
				glColor4f(PLAYER_RECTANGLE_COLOR.r, PLAYER_RECTANGLE_COLOR.g, PLAYER_RECTANGLE_COLOR.b, PLAYER_RECTANGLE_ALPHA);
				glVertex2fv(reinterpret_cast<float*>(&v1));
				glVertex2fv(reinterpret_cast<float*>(&v2));
			glEnd();
		glPopMatrix();
	}

	void WorldRenderer::RenderGameObj::operator ()(const WorldModel::ProjectileContainer::Pair &projectilePair)
	{
		static const Color projectileColors[Projectile::N_TYPES] = {Color(0.0f,0.0f,0.0f), Color(0.0f, 0.0f, 0.0f)};
		static const float projectileWidths[Projectile::N_TYPES] = {1.2f, 4.0f};
		static const float projectileAlphas[Projectile::N_TYPES] = {0.3f, 0.3f};
		
		Projectile::Type type = projectilePair.second->getType();
		Line line(projectilePair.second->getLine());

		Color color(projectileColors[type]);
		glColor4f(color.r, color.g, color.b, projectileAlphas[type]);
		WorldRenderer::renderLine(line, projectileWidths[type], projectileAlphas[type]);
	}

	void WorldRenderer::RenderGameObj::operator ()(const Explosion &explosion)
	{
		const Color &color = explosion.color;
		glColor4f(color.r, color.g, color.b, 0.3f);
		Rectangle explosionRectangle(explosion.pos, explosion.size);
		WorldRenderer::renderRectangle(explosionRectangle, GL_QUADS);
	}
	

};

#endif
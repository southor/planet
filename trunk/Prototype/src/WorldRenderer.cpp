#ifndef __worldrenderer_cpp__
#define __worldrenderer_cpp__


#include "WorldRenderer.h"
#include "common.h"
#include <algorithm>

namespace Prototype
{

	const Vec WorldRenderer::RENDER_SIZE = Vec(400.0f / 2.0f, 300.0f * (3.0f / 4.0f));

	const Pos WorldRenderer::WORLD_CENTER_POS = Pos(280.0f / 2.0f, 170.0f * (3.0f / 4.0f));

	const float WorldRenderer::Explosion::BULLET_EXPLOSION_SIZE = 8.0f;
	const float WorldRenderer::Explosion::ROCKET_EXPLOSION_SIZE = 15.0f;
	const float WorldRenderer::Explosion::EXPLOSION_TIME = 300.0f;

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

	void WorldRenderer::projectileHit(Projectile *projectile, const Pos &hitPos)
	{
		float size;
		Color color = Color(8.0f, 0.0f, 0.0f); //Color::BLACK;

		if (projectile->getType() == Projectile::BULLET) size = Explosion::BULLET_EXPLOSION_SIZE;
		else if (projectile->getType() == Projectile::ROCKET) size = Explosion::ROCKET_EXPLOSION_SIZE;
		
		Explosion explosion = {hitPos, size, color, SDL_GetTicks()};
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
		glLineWidth(1.0f);
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

	void WorldRenderer::RenderObstacle::operator ()(const WorldModel::Obstacles::Pair &obstaclePair)
	{
		glColor3f(0.0f,0.0f,0.0f);
		WorldRenderer::renderRectangle(*(obstaclePair.second), GL_QUADS);
	}

	void WorldRenderer::RenderProjectile::operator ()(const WorldModel::Projectiles::Pair &projectilePair)
	{
		static const Color projectileColors[Projectile::N_TYPES] = {Color(0.0f,0.0f,0.0f), Color(0.0f, 0.0f, 0.0f)};
		static const float projectileWidths[Projectile::N_TYPES] = {1.2f, 4.0f};
		static const float projectileAlphas[Projectile::N_TYPES] = {0.7f, 0.7f};
		
		Projectile *projectile = projectilePair.second;
		if (projectile->render)
		{
			Projectile::Type type = projectile->getType();		
			Line line(projectile->getLine());

			Color color(projectileColors[type]);
			glColor4f(color.r, color.g, color.b, projectileAlphas[type]);
			WorldRenderer::renderLine(line, projectileWidths[type], projectileAlphas[type]);
		}
		
	}

	bool WorldRenderer::RenderExplosion::operator ()(const Explosion &explosion)
	{
		const Color &color = explosion.color;
		glColor4f(color.r, color.g, color.b, 0.8f);
		Rectangle explosionRectangle(explosion.pos, explosion.size);
		WorldRenderer::renderRectangle(explosionRectangle, GL_QUADS);
		
		return explosion.startTime + Explosion::EXPLOSION_TIME < SDL_GetTicks();
	}

};

#endif

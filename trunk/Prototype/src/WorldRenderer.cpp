#ifndef __worldrenderer_cpp__
#define __worldrenderer_cpp__


#include "WorldRenderer.h"
#include "common.h"
#include <algorithm>

namespace Prototype
{

	const Vec WorldRenderer::RENDER_SIZE = Vec(400.0f / 2.0f, 300.0f * (3.0f / 4.0f));

	const float WorldRenderer::Explosion::BULLET_EXPLOSION_SIZE = 5.0f;
	const float WorldRenderer::Explosion::ROCKET_EXPLOSION_SIZE = 20.0f;

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

	//template <class Players>
	//void WorldRenderer::render(WorldModel &worldModel, Players &players, PlayerObj *localPlayerObj, Tickf tick)
	//{
	//	glLoadIdentity();

	//	// render box around render area
	//	renderViewBox();

	//	// camera
	//	glPushMatrix();
	//		{			
	//			Rectangle renderArea(getRenderArea(localPlayerObj));
	//			Vec scaleAmount = WorldModel::WORLD_SIZE / renderArea.size;
	//			glScalef(scaleAmount.x, scaleAmount.y, 1.0f);
	//			glTranslatef(-renderArea.pos.x, -renderArea.pos.y, 0.0f);			
	//		}		
	//		
	//		//// execute camera properties
	//		//switch(renderMode)
	//		//{
	//		//case HOLE_WORLD:
	//		//	// nothing to do, we are rendering the hole world
	//		//	break;
	//		//case FOLLOW_PLAYER:
	//		//	{
	//		//		
	//		//	}
	//		//	break;
	//		//default:
	//		//	assert(false);
	//		//	break;
	//		//}

	//		// render all objects			
	//		//RenderGameObj renderGameObj(&players);
	//		RenderObstacle renderObstacle;
	//		RenderPlayerObj<Players> renderPlayerObj(&players, tick);
	//		RenderProjectile renderProjectile(tick);
	//		ForEach(worldModel.getObstacles().begin(), worldModel.getObstacles().end(), renderObstacle);
	//		ForEach(worldModel.getPlayerObjs().begin(), worldModel.getPlayerObjs().end(), renderPlayerObj);
	//		ForEach(worldModel.getProjectiles().begin(), worldModel.getProjectiles().end(), renderProjectile);			
	//		//render explosion
	//		RenderExplosion renderExplosion;
	//		ForEach(explosions.begin(), explosions.end(), renderExplosion);
	//		explosions.clear();
	//	glPopMatrix();
	//}

	void WorldRenderer::projectileHit(Projectile *projectile, const Pos &hitPos)
	{
		float size;
		Color color = Color(0.0f, 0.0f, 0.0f); //Color::BLACK;

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

	//template <class Players>
	//void WorldRenderer::RenderPlayerObj<Players>::operator ()(const WorldModel::PlayerObjs::Pair &playerObjPair)
	//{
	//	static const Color PLAYER_RECTANGLE_COLOR = Color(0.7f,0.7f,0.7f);
	//	static const float PLAYER_RECTANGLE_ALPHA = 0.5f;
	//	
	//	PlayerId playerId = playerObjPair.first;
	//	PlayerObj *playerObj = playerObjPair.second;
	//	playerObj->updateToTickData(tick);
	//	
	//	
	//	Rectangle rect;
	//	playerObj->getRectangle(rect);

	//	// render rectangle
	//	glColor4f(PLAYER_RECTANGLE_COLOR.r, PLAYER_RECTANGLE_COLOR.g, PLAYER_RECTANGLE_COLOR.b, PLAYER_RECTANGLE_ALPHA);
	//	WorldRenderer::renderRectangle(rect, GL_QUADS);
	//	
	//	// setup some vertexes
	//	Vec2f v0((rect.getTopLeft() + rect.getTopRight()) / 2.0f);
	//	Vec2f v1(rect.getBottomLeft());
	//	Vec2f v2(rect.getBottomRight());

	//	// get a player color
	//	//size_t playerId = playerObj->getPlayerId();
	//	//Color playerColor(0.7f, 0.7f, 0.7f);
	//	//if (players->isValid(playerId))
	//	//{
	//	Color playerColor = (*players)[playerId].color;
	//	//}

	//	// render triangle
	//	glPushMatrix();
	//		glTranslatef(playerObj->pos.x, playerObj->pos.y, 0.0f);
	//		glRotatef(radianToDegree(playerObj->angle.getFloat()) - 90.0f, 0.0f, 0.0f, 1.0f); 
	//		glTranslatef(-playerObj->pos.x, -playerObj->pos.y, 0.0f);
	//		glBegin(GL_TRIANGLES);
	//			glColor3fv(reinterpret_cast<float*>(&playerColor));
	//			glVertex2fv(reinterpret_cast<float*>(&v0));
	//			glColor4f(PLAYER_RECTANGLE_COLOR.r, PLAYER_RECTANGLE_COLOR.g, PLAYER_RECTANGLE_COLOR.b, PLAYER_RECTANGLE_ALPHA);
	//			glVertex2fv(reinterpret_cast<float*>(&v1));
	//			glVertex2fv(reinterpret_cast<float*>(&v2));
	//		glEnd();
	//	glPopMatrix();
	//}

	void WorldRenderer::RenderProjectile::operator ()(const WorldModel::Projectiles::Pair &projectilePair)
	{
		static const Color projectileColors[Projectile::N_TYPES] = {Color(0.0f,0.0f,0.0f), Color(0.0f, 0.0f, 0.0f)};
		static const float projectileWidths[Projectile::N_TYPES] = {1.2f, 4.0f};
		static const float projectileAlphas[Projectile::N_TYPES] = {0.7f, 0.7f};
		
		Projectile *projectile = projectilePair.second;
		if (projectile->render)
		{

			//projectile->updateToTickData(tick);
			
			Projectile::Type type = projectile->getType();		
			Line line(projectile->getLine());

			Color color(projectileColors[type]);
			glColor4f(color.r, color.g, color.b, projectileAlphas[type]);
			WorldRenderer::renderLine(line, projectileWidths[type], projectileAlphas[type]);
		}
		
	}

	void WorldRenderer::RenderExplosion::operator ()(const Explosion &explosion)
	{
		const Color &color = explosion.color;
		glColor4f(color.r, color.g, color.b, 0.8f);
		Rectangle explosionRectangle(explosion.pos, explosion.size);
		WorldRenderer::renderRectangle(explosionRectangle, GL_QUADS);
	}
	

};

#endif

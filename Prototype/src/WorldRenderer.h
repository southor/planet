#ifndef __worldrenderer_h__
#define __worldrenderer_h__

#include "WorldModel.h"
#include "Player.h"
#include "common.h"

namespace Prototype
{
	class WorldRenderer
	{
	public:
		
		enum RenderMode
		{
			HOLE_WORLD,
			WORLD_CENTER,
			FOLLOW_PLAYER
		};

		static const Vec RENDER_SIZE;

		static const Pos WORLD_CENTER_POS; // (for rendering the server)

		inline Rectangle getRenderArea(PlayerObj *centeredPlayerObj)
		{
			if (renderMode == HOLE_WORLD)
			{
				return Rectangle(Pos(0.0f, 0.0f), WorldModel::WORLD_SIZE);
			}
			else if (renderMode == WORLD_CENTER)
			{
				Rectangle rectangle(WORLD_CENTER_POS, RENDER_SIZE);
				return rectangle;
			}
			else if (renderMode == FOLLOW_PLAYER)
			{
				assert(centeredPlayerObj);
				// setting center pos
				Pos tmpPos;
				Rectangle rectangle(tmpPos, RENDER_SIZE);
				rectangle.setCenterPos(centeredPlayerObj->pos);
				return rectangle;
			}
			else
			{
				assert(false);
				return Rectangle(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		WorldRenderer(RenderMode renderMode);

		void setupProjection();
		
		template <class Players>
		void render(WorldModel &worldModel, Players &players, PlayerObj *centeredPlayerObj)//, Tickf tick)
		{
			glLoadIdentity();

			// render box around render area
			renderViewBox();

			// camera
			glPushMatrix();
				{
					//centeredPlayerObj->updateToTickData(tick);
					Rectangle renderArea(getRenderArea(centeredPlayerObj));
					Vec scaleAmount = WorldModel::WORLD_SIZE / renderArea.size;
					glScalef(scaleAmount.x, scaleAmount.y, 1.0f);
					glTranslatef(-renderArea.pos.x, -renderArea.pos.y, 0.0f);			
				}		

				// render all objects			
				RenderObstacle renderObstacle;
				RenderPlayerObj<Players> renderPlayerObj(&players);
				RenderProjectile renderProjectile;
				WorldModel::Projectiles &projectiles = worldModel.getProjectiles();
				ForEach(worldModel.getObstacles().begin(), worldModel.getObstacles().end(), renderObstacle);
				ForEach(worldModel.getPlayerObjs().begin(), worldModel.getPlayerObjs().end(), renderPlayerObj);
				ForEach(projectiles.begin(), projectiles.end(), renderProjectile);

				//render explosion
				RenderExplosion renderExplosion;
				for (size_t i = 0; i < explosions.size(); i++)
				{
					Explosion explosion = explosions[i];
					
					bool remove = renderExplosion(explosion);

					if (remove)
						explosions.erase(explosions.begin() + i);
				}
			glPopMatrix();
		}
		
		
		void projectileHit(Projectile *projectile, const Pos &hitPos);

		static void renderRectangle(const Rectangle &rect, GLenum mode);
		static void renderLine(const Line &line, float lineWidth, float alpha);

	private:

		RenderMode renderMode;

		struct Explosion
		{
			static const float BULLET_EXPLOSION_SIZE;
			static const float ROCKET_EXPLOSION_SIZE;
			static const float EXPLOSION_TIME;

			Pos pos;
			float size;
			Color color;
			float startTime;
		};
		std::vector<Explosion> explosions;
		
		class RenderObstacle
		{
		public:
			void operator ()(const WorldModel::Obstacles::Pair &obstaclePair);
		};

		template <class Players>
		class RenderPlayerObj
		{
		private:
			Players *players;
		public:
			
			inline RenderPlayerObj(Players *players) : players(players)
			{}

			void operator ()(const WorldModel::PlayerObjs::Pair &playerObjPair)
			{
				static const Color PLAYER_RECTANGLE_COLOR = Color(0.7f,0.7f,0.7f);
				static const float PLAYER_RECTANGLE_ALPHA = 0.5f;
				
				PlayerId playerId = playerObjPair.first;
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

				Color playerColor = (*players)[playerId]->color;

				// render triangle
				glPushMatrix();
					glTranslatef(playerObj->pos.x, playerObj->pos.y, 0.0f);
					glRotatef(radianToDegree(playerObj->angle.getFloat()) - 90.0f, 0.0f, 0.0f, 1.0f); 
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
		};

		class RenderProjectile
		{
		public:
			void operator ()(const WorldModel::Projectiles::Pair &projectilePair);			
		};

		class RenderExplosion
		{
		public:
			// visual effects
			bool operator ()(const Explosion &explosion);
		};

		void renderViewBox();

	};
};

#endif

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
			FOLLOW_PLAYER
		};

		static const Vec RENDER_SIZE;

		inline Rectangle getRenderArea(PlayerObj *localPlayerObj)
		{
			if (renderMode == HOLE_WORLD)
			{
				return Rectangle(Pos(0.0f, 0.0f), WorldModel::WORLD_SIZE);
			}
			else if (renderMode == FOLLOW_PLAYER)
			{
				// setting center pos
				Pos tmpPos;
				Rectangle rectangle(tmpPos, RENDER_SIZE);
				rectangle.setCenterPos(localPlayerObj->pos);
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
		void render(WorldModel &worldModel, Players &players, PlayerObj *localPlayerObj);
		void projectileHit(Projectile *projectile, const Pos &hitPos);

		static void renderRectangle(const Rectangle &rect, GLenum mode);
		static void renderLine(const Line &line, float lineWidth, float alpha);

	private:

		RenderMode renderMode;

		struct Explosion
		{
			static const float BULLET_EXPLOSION_SIZE;
			static const float ROCKET_EXPLOSION_SIZE;

			Pos pos;
			float size;
			Color color;
		};
		std::vector<Explosion> explosions;
		

		// Functor for rendering a game object
		class RenderGameObj
		{
		private:
			Players *players;
		public:
			
			RenderGameObj(Players *players) : players(players)
			{}

			// game objects
			void operator ()(const WorldModel::ObstacleContainer::Pair &obstaclePair);
			void operator ()(const WorldModel::PlayerObjContainer::Pair &playerObjPair);
			void operator ()(const WorldModel::ProjectileContainer::Pair &projectilePair);
			
			// visual effects
			void operator ()(const Explosion &explosion);
		};



		void renderViewBox();

	};
};

#endif

#ifndef __worldrenderer_h__
#define __worldrenderer_h__

#include "worldmodel.h"
#include "Player.h"
#include "common.h"

namespace Prototype
{
	class WorldRenderer
	{
	public:
		
		enum RenderMode
		{
			HOLE_WORLD
		};

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
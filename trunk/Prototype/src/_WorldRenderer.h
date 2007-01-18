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

		static void renderRectangle(const Rectangle &rect, GLenum mode);
		static void renderLine(const Line &line, float lineWidth, float alpha);

	private:

		RenderMode renderMode;
		

		// Functor for rendering a game object
		class RenderGameObj
		{
		private:
			Players *players;
		public:
			
			RenderGameObj(Players *players) : players(players)
			{}

			//void operator ()(const Obstacle* obstacle);
			//void operator ()(const PlayerObj* playerObj);
			//void operator ()(const Projectile* projectile);
			void operator ()(const WorldModel::ObstacleContainer::Pair &obstaclePair);
			void operator ()(const WorldModel::PlayerObjContainer::Pair &playerObjPair);
			void operator ()(const WorldModel::ProjectileContainer::Pair &projectilePair);
		};



		void renderViewBox();

	};
};

#endif
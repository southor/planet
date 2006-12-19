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

			void operator ()(const Obstacle* obstacle);
			void operator ()(const PlayerObj* playerObj);
			void operator ()(const Projectile* projectile);
		};

		static void renderRectangle(const Rectangle &rect, GLenum mode);

		void renderViewBox();

	};
};

#endif
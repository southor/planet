#ifndef __worldrenderer_h__
#define __worldrenderer_h__

#include "worldmodel.h"

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

		void render(WorldModel &worldModel, PlayerObj *currentPlayer);

	private:

		RenderMode renderMode;

		// Functor for rendering a game object
		class RenderGameObj
		{
		private:
			PlayerObj *currentPlayerObj;
		public:
			RenderGameObj(PlayerObj *currentPlayerObj) : currentPlayerObj(currentPlayerObj)
			{}

			void operator ()(const Obstacle* obstacle);
			void operator ()(const PlayerObj* playerObj);
			void operator ()(const Projectile* projectile);
		};
	};
};

#endif
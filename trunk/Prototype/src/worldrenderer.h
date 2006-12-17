#ifndef __worldrenderer_h__
#define __worldrenderer_h__

#include "worldmodel.h"
#include "Player.h"

namespace Prototype
{
	class WorldRenderer
	{
	public:
		
		enum RenderMode
		{
			HOLE_WORLD
		};

		WorldRenderer(RenderMode renderMode, const Color &playerColor);

		void render(WorldModel &worldModel, Players &players, PlayerObj *currentPlayer);

	private:

		typedef std::vector<Color> PlayerColors;

		RenderMode renderMode;
		PlayerColors playerColors;
		

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

	public:

		inline void setPlayerColor(int playerId, Color &color)
		{
			playerColors[playerId] = color;
		}

	};
};

#endif
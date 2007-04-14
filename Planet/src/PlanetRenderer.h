#ifndef _PlanetRenderer_h_
#define _PlanetRenderer_h_

#include "LaserBeamRenderHandler.h"
#include "LaserBeamVertexes.h"

namespace Planet
{
	class PlanetRenderer
	{
	public:

		PlanetRenderer()
		{
		}

		void render(Planet &planet)
		{
			planet.getPlanetBody()->render();

			// Disable lights for ship and sight rendering
			glDisable(GL_LIGHTING);

			Planet::PlayerObjs::Iterator playerObjsIt = planet.getPlayerObjs().begin();
			Planet::PlayerObjs::Iterator playerObjsEnd = planet.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{
				playerObjsIt->second->getShip()->render();
			}
		}
		
		void render(Pos pos, Vec direction)
		{
		}

	};
};

#endif


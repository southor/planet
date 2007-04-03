#ifndef _PlanetRenderer_h_
#define _PlanetRenderer_h_

#include "LaserBeamRenderHandler.h"
#include "LaserBeamVertexes.h"

namespace Planet
{
	class PlanetRenderer
	{
	public:
		//LaserBeamRenderHandler laserBeamRenderHandler;
		//LaserBeamVertexes laserBeam;

		PlanetRenderer()
		{
			/*
			Line line(Pos(10.0f, 10.0f, 10.0f), Vec(10.0f, 10.0f, 10.0f));
			laserBeam.setVecs(line, 1.0f);
			laserBeam.setColors(Color::RED, 1.0f, false);

			laserBeamRenderHandler.addLaserBeam(&laserBeam);
			*/
		}

		void render(Planet &planet)
		{
			planet.getPlanetBody()->render();

			// Disable lights for ship and sight rendering
			glDisable(GL_LIGHTING);
			//glDisable(GL_DEPTH_TEST);

			Planet::PlayerObjs::Iterator playerObjsIt = planet.getPlayerObjs().begin();
			Planet::PlayerObjs::Iterator playerObjsEnd = planet.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{
				playerObjsIt->second->getShip()->render();
			}
		}
		
		void render(Pos pos, Vec direction)
		{
			/*
			laserBeam.setVecs(Line(pos, direction), 0.5f);

			laserBeamRenderHandler.setupRender();
			laserBeamRenderHandler.render();
			*/
		}

	};
};

#endif


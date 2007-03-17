#include "LaserBeamVertexes.h"

namespace Planet
{
	void LaserBeamVertexes::updateVecs()
	{
		if (!isMember()) return;
		
		LaserBeamVecs &laserBeamVecs = data1();
		
		Vec4f startPos(line.pos, 1.0f);
		Vec4f endPos(line.getEndPos(), 1.0f);

		// set vertexes
		for(int i=0; i<4*3; i += 4)
		{
			laserBeamVecs.pSides[i] = startPos;
			laserBeamVecs.pSides[i+1] = startPos;
			laserBeamVecs.pSides[i+2] = endPos;
			laserBeamVecs.pSides[i+3] = endPos;

			laserBeamVecs.nSides[i] = startPos;
			laserBeamVecs.nSides[i+1] = startPos;
			laserBeamVecs.nSides[i+2] = endPos;
			laserBeamVecs.nSides[i+3] = endPos;
		}

		// modify vertexes width width
		laserBeamVecs.pSides[0+1].x += width;
		laserBeamVecs.pSides[0+2].x += width;
		laserBeamVecs.pSides[4+1].y += width;
		laserBeamVecs.pSides[4+2].y += width;
		laserBeamVecs.pSides[8+1].z += width;
		laserBeamVecs.pSides[8+2].z += width;

		laserBeamVecs.nSides[0+1].x -= width;
		laserBeamVecs.nSides[0+2].x -= width;
		laserBeamVecs.nSides[4+1].y -= width;
		laserBeamVecs.nSides[4+2].y -= width;
		laserBeamVecs.nSides[8+1].z -= width;
		laserBeamVecs.nSides[8+2].z -= width;
	}

	void LaserBeamVertexes::updateColors()
	{
		if (!isMember()) return;
		
		LaserBeamColors &laserBeamColors = data2();

		Vec4f startColor(color.r, color.g, color.b, intensity);
		Vec4f endColor(color.r, color.g, color.b, trailMode ? 0.0f : intensity);

		// set colors
		for(int i=0; i<4*3; i += 4)
		{
			laserBeamColors.pSides[i] = startColor;
			laserBeamColors.pSides[i+1] = startColor;
			laserBeamColors.pSides[i+2] = endColor;
			laserBeamColors.pSides[i+3] = endColor;

			laserBeamColors.nSides[i] = startColor;
			laserBeamColors.nSides[i+1] = startColor;
			laserBeamColors.nSides[i+2] = endColor;
			laserBeamColors.nSides[i+3] = endColor;
		}

		// modify alphas, (faded in the edges)
		for(int i=0; i<4*3; i += 4)
		{
			laserBeamColors.pSides[i+1].w = 0.0f;
			laserBeamColors.pSides[i+2].w = 0.0f;

			laserBeamColors.nSides[i+1].w = 0.0f;
			laserBeamColors.nSides[i+2].w = 0.0f;
		}

	}

};


#ifndef _LaserBeamRenderHandler_h_
#define _LaserBeamRenderHandler_h_

#include "LaserBeamVertexes.h"

namespace Planet
{

	class LaserBeamRenderHandler
	{
	private:

		ExternLB externLB;


		inline const LaserBeamVecs *getVtxArr() const		{ return externLB.getData1Arr(); }

		inline const LaserBeamColors *getColorArr() const	{ return externLB.getData2Arr(); }

	public:
		
		LaserBeamRenderHandler(size_t initCapacity = 0, float growFactor = ExternLB::DEFAULT_GROW_FACTOR);		

		virtual ~LaserBeamRenderHandler();


		inline size_t getSize() const 						{ return externLB.getSize(); }

		float getGrowFactor() const							{ return externLB.getGrowFactor(); }

		void setGrowFactor(float growFactor)				{ externLB.setGrowFactor(growFactor); }

		void addLaserBeam(LaserBeamVertexes *laserBeam)		{ externLB.add(laserBeam); }

		void removeLaserBeam(LaserBeamVertexes *laserBeam)	{ externLB.remove(laserBeam); }

		inline void removeAllLaserBeams()					{ externLB.clear(); }

		virtual void setupRender();

		void render();

		//debug
		bool isConsistent()
		{
			return externLB.isConsistent();
		}
	};
};

#endif
#ifndef _LaserBeamVertexes_h_
#define _LaserBeamVertexes_h_

#include "externdata.h"
#include "vec4f.h"
#include "Line.h"
#include "Color.h"

namespace Planet
{

	struct LaserBeamVecs
	{
		Vec4f pSides[4*3];
		Vec4f nSides[4*3];		
	};

	typedef LaserBeamVecs LaserBeamColors;
	

	typedef Extern2Data<LaserBeamVecs, LaserBeamColors> ExternLB;

	class LaserBeamVertexes : public ExternLB::Accesser
	{
	private:

		Line line;

		float width;
		Color color;
		float intensity;
		bool trailMode;

		void updateVecs();
		void updateColors();
	
	protected:

		// called when the object was added to a extern2data
		void added()
		{
			void updateVecs();
			void updateColors();
		}

	public:

		LaserBeamVertexes()	{}

		const Line& getLine() const					{ return line; }		
		float getWidth() const						{ return width; }
		const Color& getColor() const				{ return color; }
		float getIntensity() const					{ return intensity; }
		bool getTrailMode() const					{ return trailMode; }


		// set line
		void setLine(const Line &line)
		{
			this->line = line;
			updateVecs();
		}
		
		// set width
		void setWidth(float width)
		{
			this->width = width;
			updateVecs();
		}

		// set both line and width
		void setVecs(const Line &line, float width)
		{
			this->line = line;
			this->width = width;
			updateVecs();
		}

		// set color
		void setColor(const Color &color)
		{
			this->color = color;
			updateColors();
		}

		// set intensity (alpha value)
		void setIntensity(float intensity)
		{
			this->intensity = intensity;
			updateColors();
		}

		// set trail mode
		void setTrailMode(bool trailMode)
		{
			this->trailMode = trailMode;
			updateColors();
		}

		// set color, intensity (alpha value) and trail mode
		void setTrailMode(const Color &color, float intensity, bool trailMode)
		{
			this->color = color;
			this->intensity = intensity;
			this->trailMode = trailMode;
			updateColors();
		}

	};

};

#endif
#include "line.h"
#include "rectangle.h"

namespace Prototype
{
	float Line::crossPoint(const Line &line)
	{
		//if (ext == line.ext) return 2.0f;


		if (line.ext.x == 0.0f)
		{
			if (ext.x == 0.0f) return 2.0f;
			float k = ext.y / ext.x;
			float xDiff = line.pos.x - pos.x;
			float yCross = xDiff;
		}




		

		float posXDiff = this->pos.x - line.pos.x;
		
		if ((line.ext.x == 0.0f) && ((posXDiff == 0.0f) || (this->ext.x == 0.0f))) return 2.0f;

		float k = line.ext.y / line.ext.x;

		
		float s = (k*posXDiff + line.pos.y - this->pos.y) /
					(this->ext.y - k * this->ext.x);

		float t = (posXDiff + this->ext.x * s) / line.ext.x;

		if (!line.crossing(t)) return 2.0f;

		return s;
	}

	float Line::minCrossPoint(const Rectangle &rectangle)
	{
		static const float OUTSIDE = 2.0f;
		
		float leftCrossPoint = crossPoint(rectangle.getLeftLine());
		float rightCrossPoint = crossPoint(rectangle.getRightLine());
		float topCrossPoint = crossPoint(rectangle.getTopLine());
		float bottomCrossPoint = crossPoint(rectangle.getBottomLine());

		//// debug temp
		//topCrossPoint = OUTSIDE;
		//bottomCrossPoint = OUTSIDE;
		
		leftCrossPoint = (leftCrossPoint >= 0.0f) ? leftCrossPoint : OUTSIDE;
		rightCrossPoint = (rightCrossPoint >= 0.0f) ? rightCrossPoint : OUTSIDE;
		topCrossPoint = (topCrossPoint >= 0.0f) ? topCrossPoint : OUTSIDE;
		bottomCrossPoint = (bottomCrossPoint >= 0.0f) ? bottomCrossPoint : OUTSIDE;

		return tmin(leftCrossPoint, rightCrossPoint, topCrossPoint, bottomCrossPoint);
	}

	bool Line::rightSide(const Pos &pos)
	{
		Vec v(pos - this->pos);
		
		//float k = ext.y / ext.x;
		//float vk = v.y / v.x;	
		//if (ext.x > 0.0f)
		//{
		//	if (v.x > 0.0f)
		//	{
		//		return vk) <= k;
		//	}
		//	else if (v.x < 0.0f)
		//	{
		//		return vk >= k;
		//	}
		//	else
		//	{
		//		return v.y < 0.0f;
		//	}
		//}
		//else if (ext.x < 0.0f)
		//{
		//	if (v.x > 0.0f)
		//	{
		//		return vk) <= k;
		//	}
		//	else if (v.x < 0.0f)
		//	{
		//		return vk) >= k;
		//	}
		//	else
		//	{
		//		return v.y > 0.0f;
		//	}
		//}
		//else
		//{
		//	if (ext.y > 0.0f) return v.x >= 0.0f;
		//	else return v.x <= 0.0f;
		//}
		
		if (ext.x == 0.0f)
		{
			if (ext.y > 0.0f) return v.x >= 0.0f;
			else return v.x <= 0.0f;
		}
		else
		{
			float k = ext.y / ext.x;
			float vk = v.y / v.x;

			if (v.x > 0.0f) return vk <= k;
			else if (v.x < 0.0f) return vk >= k;
			else return v.y < 0.0f;
		}
	}

	bool Line::crossingRectangle(const Rectangle &rectangle)
	{
		bool bottomLeftRightOf = rightSide(rectangle.getTopLeft());
		if ((bottomLeftRightOf != rightSide(rectangle.getTopRight())) ||
			(bottomLeftRightOf != rightSide(rectangle.getBottomLeft())) ||
			(bottomLeftRightOf != rightSide(rectangle.getBottomRight())))
		{
			// extended line is crossing box
			
			if (rectangle.contains(pos) || rectangle.contains(getEndPos())) return true;
			
			Line line;
			int nCrossedBoxLines = 0;
			
			line = rectangle.getLeftLine();			
			if (line.rightSide(pos) != line.rightSide(getEndPos())) ++nCrossedBoxLines;

			line = rectangle.getRightLine();			
			if (line.rightSide(pos) != line.rightSide(getEndPos())) ++nCrossedBoxLines;

			line = rectangle.getTopLine();			
			if (line.rightSide(pos) != line.rightSide(getEndPos())) ++nCrossedBoxLines;

			line = rectangle.getBottomLine();			
			if (line.rightSide(pos) != line.rightSide(getEndPos())) ++nCrossedBoxLines;

			return nCrossedBoxLines >= 2;
		}
		return false;			
	}

};

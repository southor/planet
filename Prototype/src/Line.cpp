#include "Line.h"
#include "Rectangle.h"

namespace Prototype
{
	float Line::crossPoint(OrthogonalLine line)
	{
		float OUTSIDE = 2.0f;
		if (line.axis == OrthogonalLine::X_AXIS)
		{
			if (ext.y == 0.0f) return OUTSIDE;
			float yDiff = line.pos.y - pos.y;
			float s = yDiff / ext.y;
			
			float t = (getPosAlong(s).x - line.pos.x) / line.ext;
			if ((t >= 0.0f) && (t <= 1.0f)) return s;
			else return OUTSIDE;
		}
		else
		{
			assert(line.axis == OrthogonalLine::Y_AXIS);

			if (ext.x == 0.0f) return OUTSIDE;
			float xDiff = line.pos.x - pos.x;
			float s = xDiff / ext.x;
			
			float t = (getPosAlong(s).y - line.pos.y) / line.ext;
			if ((t >= 0.0f) && (t <= 1.0f)) return s;
			else return OUTSIDE;
		}
	}

	float Line::minCrossPoint(const Rectangle &rectangle)
	{
		static const float OUTSIDE = 2.0f;

		float leftCrossPoint = crossPoint(rectangle.getLeftLine());
		float rightCrossPoint = crossPoint(rectangle.getRightLine());
		float topCrossPoint = crossPoint(rectangle.getTopLine());
		float bottomCrossPoint = crossPoint(rectangle.getBottomLine());

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


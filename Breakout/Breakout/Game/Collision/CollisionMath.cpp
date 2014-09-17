#include "CollisionMath.h"

bool CollisionMath::BoxBoxIntersection(BoundingBox2 _boundingBox1, BoundingBox2 _boundingBox2) const
{
	return (abs(_boundingBox1.x - _boundingBox2.x) * 2 < (_boundingBox1.width + _boundingBox2.width))
		&& (abs(_boundingBox1.y - _boundingBox2.y) * 2 < (_boundingBox1.height + _boundingBox2.height));
}

bool CollisionMath::BoxBoxIntersection(BoundingBox2& _boundingBox1, BoundingBox2 _boundingBox2, Vector2 _boundingBox1Velocity) const
{
	bool isColliding = (abs(_boundingBox1.x - _boundingBox2.x) * 2 < (_boundingBox1.width + _boundingBox2.width)) 
		&& (abs(_boundingBox1.y - _boundingBox2.y) * 2 < (_boundingBox1.height + _boundingBox2.height));
	
	if (!isColliding)
		return isColliding;

	Vector2 intersectionCorner;
	// Check which corner(s) of bounding box 1 that has collided with bounding box 2
	// Top left corner
	if (PointBoxIntersection(Vector2(_boundingBox1.x, _boundingBox1.y), _boundingBox2))
		intersectionCorner = Vector2(_boundingBox1.x, _boundingBox1.y);
	// Top right corner
	else if (PointBoxIntersection(Vector2(_boundingBox1.x + _boundingBox1.width, _boundingBox1.y), _boundingBox2))
		intersectionCorner = Vector2(_boundingBox1.x + _boundingBox1.width, _boundingBox1.y);
	// Bottom left corner
	else if (PointBoxIntersection(Vector2(_boundingBox1.x, _boundingBox1.y + _boundingBox1.height), _boundingBox2))
		intersectionCorner = Vector2(_boundingBox1.x, _boundingBox1.y + _boundingBox1.height);
	// Bottom right corner
	else
		intersectionCorner = Vector2(_boundingBox1.x + _boundingBox1.width, _boundingBox1.y + _boundingBox1.height);

	// Need the velocity to be normalized in order to move box 1 in the right direction
	Vector2 normVelocity = -_boundingBox1Velocity.Normalize();

	// ComparisonPoint is the corner that will be used to compare the position of box 1 and box 2 when box 1 will be pushed out of box 2
	// ComparisonPointEndPointXAxis is the end point of the line that goes from comparison point to the corner that is either on the left or right side
	// ComparisonPointEndPointYAxis is the end point of the line that goes from comparison point to the corner that is either on the upper or lower side
	Vector2 box2ComparisonPoint, box2CPEndPointXAxis, box2CPEndPointYAxis, intersectionPoint1, intersectionPoint2;

	// Top left corner
	if (normVelocity.x <= 0 && normVelocity.y <= 0)
	{
		// Top left
		box2ComparisonPoint = Vector2(_boundingBox2.x, _boundingBox2.y);
		// Top right
		box2CPEndPointXAxis = Vector2(_boundingBox2.x + _boundingBox2.width, _boundingBox2.y);
		// Bottom left
		box2CPEndPointYAxis = Vector2(_boundingBox2.x, _boundingBox2.y + _boundingBox2.height);
	}
	// Top right corner
	else if (normVelocity.x > 0 && normVelocity.y <= 0)
	{
		// Top right
		box2ComparisonPoint = Vector2(_boundingBox2.x + _boundingBox2.width, _boundingBox2.y);
		// Top left
		box2CPEndPointXAxis = Vector2(_boundingBox2.x, _boundingBox2.y);
		// Bottom right
		box2CPEndPointYAxis = Vector2(_boundingBox2.x + _boundingBox2.width, _boundingBox2.y + _boundingBox2.height);
	}
	// Bottom left corner
	else if (normVelocity.x <= 0 && normVelocity.y > 0)
	{
		// Bottom left
		box2ComparisonPoint = Vector2(_boundingBox2.x, _boundingBox2.y + _boundingBox2.height);
		// Bottom right
		box2CPEndPointXAxis = Vector2(_boundingBox2.x + _boundingBox2.width, _boundingBox2.y + _boundingBox2.height);
		// Top left
		box2CPEndPointYAxis = Vector2(_boundingBox2.x, _boundingBox2.y);
	}
	// Bottom right corner
	else
	{
		// Bottom right
		box2ComparisonPoint = Vector2(_boundingBox2.x + _boundingBox2.width, _boundingBox2.y + _boundingBox2.height);
		// Bottom left
		box2CPEndPointXAxis = Vector2(_boundingBox2.x, _boundingBox2.y + _boundingBox2.height);
		// Top right
		box2CPEndPointYAxis = Vector2(_boundingBox2.x + _boundingBox2.width, _boundingBox2.y);
	}

	Vector2 intersectionPointXAxis, intersectionPointYAxis;
	bool xAxisIntersection, yAxisIntersection;

	// Check if the corner that is being moved toward the reverse velocity intersects with either of the axis
	xAxisIntersection = LineLineIntersection(intersectionCorner, -_boundingBox1Velocity, box2ComparisonPoint, box2CPEndPointXAxis, intersectionPointXAxis);
	yAxisIntersection = LineLineIntersection(intersectionCorner, -_boundingBox1Velocity, box2ComparisonPoint, box2CPEndPointYAxis, intersectionPointYAxis);

	float distance;
	// If the line intersects both axis then we need to know the closest intersection point of those two
	if (xAxisIntersection && yAxisIntersection)
	{
		float distanceToXAxis = intersectionCorner.Distance(intersectionPointXAxis);
		float distanceToYAxis = intersectionCorner.Distance(intersectionPointYAxis);
		if (distanceToXAxis < distanceToYAxis)
			distance = distanceToXAxis;
		else
			distance = distanceToYAxis;
	}
	else if (xAxisIntersection)
		distance = intersectionCorner.Distance(intersectionPointXAxis);
	else if (yAxisIntersection)
		distance = intersectionCorner.Distance(intersectionPointYAxis);
	else
		printf("Something went terribly wrong at the box to box intersection check, do you even math?");

	_bb1NewPosition = Vector2(_boundingBox1.x + normVelocity.x * distance, _boundingBox1.y + normVelocity.y * distance);

	return isColliding;
}

bool CollisionMath::PointBoxIntersection(Vector2 _point, BoundingBox2 _boundingBox) const
{
	if (_point.x > _boundingBox.x + _boundingBox.width)		return false;
	if (_point.x < _boundingBox.x)							return false;
	if (_point.y > _boundingBox.y + _boundingBox.height)	return false;
	if (_point.y < _boundingBox.y)							return false;
	return true;
}

bool CollisionMath::LineLineIntersection(Vector2 _startPoint1, Vector2 _endPoint1, Vector2 _startPoint2, Vector2 _endPoint2, Vector2& _intersectionPoint) const
{
	Vector2 x = _startPoint2 - _startPoint1;
	Vector2 d1 = _endPoint1 - _startPoint1;
	Vector2 d2 = _endPoint2 - _startPoint2;

	float cross = d1.x*d2.y - d1.y*d2.x;
	if (abs(cross) < /*EPS*/1e-8)
		return false;

	float t1 = (x.x * d2.y - x.y * d2.x) / cross;
	_intersectionPoint = _startPoint1 + d1 * t1;
	return true;
}
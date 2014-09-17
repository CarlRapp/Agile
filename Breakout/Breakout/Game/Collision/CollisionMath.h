#ifndef _COLLISIONMATH_H_
#define _COLLISIONMATH_H_

#include "../../stdafx.h"

class CollisionMath
{
private:
public:
	bool BoxBoxIntersection(BoundingBox2 _boundingBox1, BoundingBox2 _boundingBox2) const;
	bool BoxBoxIntersection(BoundingBox2& _boundingBox1, BoundingBox2 _boundingBox2, Vector2 _boundingBox1Velocity) const;
	bool SphereBoxIntersection(Sphere2 _sphere, BoundingBox2 _boundingBox) const;
	bool SphereBoxIntersection(Sphere2& _sphere, BoundingBox2 _boundingBox) const;
	bool PointBoxIntersection(Vector2 _point, BoundingBox2 _boundingBox) const;
	bool LineLineIntersection(Vector2 _startPoint1, Vector2 _endPoint1, Vector2 _startPoint2, Vector2 _endPoint2, Vector2& _intersectionPoint) const;
};

#endif
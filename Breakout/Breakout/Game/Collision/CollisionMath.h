#ifndef _COLLISIONMATH_H_
#define _COLLISIONMATH_H_

#include "../../stdafx.h"

class CollisionMath
{
private:
public:
	// Checks if a box intersects with a box
	inline bool BoxBoxIntersection(const BoundingBox& _boundingBox1, const BoundingBox& _boundingBox2) const;

	// Checks if a box intersects with a box. If they do intersect, give the first box a new position that doesn't intersect with the second box
	bool BoxBoxIntersection(BoundingBox& _boundingBox1, const BoundingBox& _boundingBox2, const Vector2& _boundingBox1Velocity) const;

	// Checks if a sphere intersects with a box
	inline bool SphereBoxIntersection(const BoundingSphere& _sphere, const BoundingBox& _boundingBox) const;

	// Checks if a sphere intersects with a box. If they do intersect, give the sphere a new position that doesn't intersect with the box
	bool SphereBoxIntersection(BoundingSphere& _sphere, const BoundingBox& _boundingBox, const Vector2& _sphereVelocity) const;

	// Checks if a point intersects with a box
	inline bool PointBoxIntersection(const Vector2& _point, const BoundingBox& _boundingBox) const;

	// Checks if a point intersects with a sphere
	inline bool PointSphereIntersection(const Vector2& _point, const BoundingSphere& _sphere) const;

	// Checks if two lines intersect. If they do intersect, return the intersection point
	bool LineLineIntersection(const Vector2& _startPoint1, const Vector2& _endPoint1, const Vector2& _startPoint2, const Vector2& _endPoint2, Vector2& _intersectionPoint) const;
};

#endif
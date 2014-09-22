#ifndef _COLLISIONMATH_H_
#define _COLLISIONMATH_H_

#include "../../stdafx.h"
#include "../../ComponentSystem/Component/BoxComponent.h"
#include "../../ComponentSystem/Component/SphereComponent.h"

class CollisionMath
{
private:
public:
	// Checks if a box intersects with a box
	static inline bool BoxBoxIntersection(const BoxComponent& _boundingBox1, const BoxComponent& _boundingBox2);

	// Checks if a box intersects with a box. If they do intersect, give the first box a new position that doesn't intersect with the second box
	static bool BoxBoxIntersection(BoxComponent& _boundingBox1, const BoxComponent& _boundingBox2, const VECTOR2& _boundingBox1Velocity);

	// Checks if a sphere intersects with a box
	static inline bool SphereBoxIntersection(const SphereComponent& _sphere, const BoxComponent& _boundingBox);

	// Checks if a sphere intersects with a box. If they do intersect, give the sphere a new position that doesn't intersect with the box
	static bool SphereBoxIntersection(SphereComponent& _sphere, const BoxComponent& _boundingBox, const VECTOR2& _sphereVelocity);

	// Checks if a point intersects with a box
	static inline bool PointBoxIntersection(const VECTOR2& _point, const BoxComponent& _boundingBox);

	// Checks if a point intersects with a sphere
	static inline bool PointSphereIntersection(const VECTOR2& _point, const SphereComponent& _sphere);

	// Checks if two lines intersect. If they do intersect, return the intersection point
	static bool LineLineIntersection(VECTOR2& _intersectionPoint, const VECTOR2& _startPoint1, const VECTOR2& _endPoint1, const VECTOR2& _startPoint2, const VECTOR2& _endPoint2);
};

#endif
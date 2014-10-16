#ifndef _BOUNDINGBOX2D_H_
#define _BOUNDINGBOX2D_H_

#include "MathHelper.h"


struct BoundingBox2D
{
	VECTOR2 Min, Max;

	BoundingBox2D(VECTOR2 _min, VECTOR2 _max) 
		: Min(_min), Max(_max) { };

	BoundingBox2D(float _minX, float _minY, float _maxX, float _maxY) 
		: Min(VECTOR2(_minX, _minY)), Max(VECTOR2(_maxX, _maxY)) { };

	bool Intersects(VECTOR2 _point)
	{
		return _point.x > Min.x && _point.y > Min.y
			&& _point.x < Max.x && _point.y < Max.y;
	}

};

#endif
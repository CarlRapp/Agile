#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <string>

struct Vector2;
struct Vector3;

struct Vector2
{
public:
	float X, Y;

	Vector2(float x, float y) : X(x), Y(y) {}
};

struct Vector3
{
public:
	float X, Y, Z;

	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
	Vector3(Vector2 vector, float z) : X(vector.X), Y(vector.Y), Z(z) {}

	Vector2 ToVector2() { return Vector2(X, Y); }
};

#endif
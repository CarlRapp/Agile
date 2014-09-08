#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "../stdafx.h"

struct Vertex
{
public:
	Vertex(Vector3 position, Vector2 texture, Vector3 normal) : Position(position), Texture(texture), Normal(normal) {}

	Vector3 Position;
	Vector2 Texture;
	Vector3 Normal;
};


#endif
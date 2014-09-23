#pragma once
#ifndef MODELHELPER_H
#define MODELHELPER_H	

#include <string>
#include <vector>

#include "../stdafx.h"

using namespace std;
struct Material
{
	string			Name;
	unsigned int	Illum;
	float			Ka[3];
	float			Kd[3];
	float			Ks[3];
	float			Tf[3];
	float			Ns;
	float			Ni;
	string			Map_Ka;
	string			Map_Kd;
	string			Map_Ks;
	string			Map_Ns;
	string			Map_Tf;
	string			Map_bump;

	Material()
	{
		Illum = 0;
		Ns = 0;
		Ni = 0;
		for (int i = 0; i < 3; i++)
		{
			Ka[i] = 0.2f;
			Kd[i] = 0.8f;
			Ks[i] = 1.0;
			Tf[i] = 0;
		}
		Name		= "none";
		Map_Ka		= "none";
		Map_Kd		= "none";
		Map_Ks		= "none";
		Map_Ns		= "none";
		Map_Tf		= "none";
		Map_bump	= "none";
	}
};

struct Vertex
{
public:
	Vertex() : Position(VECTOR3(0, 0, 0)), Texture(VECTOR2(0, 0)), Normal(VECTOR3(0, 0, 0)) {}
	Vertex(VECTOR3 position, VECTOR2 texture, VECTOR3 normal) : Position(position), Texture(texture), Normal(normal) {}

	VECTOR3			Position;
	VECTOR2			Texture;
	VECTOR3			Normal;
};

struct Triangle
{
	Triangle() {}
	Triangle(Vertex vertex1, Vertex vertex2, Vertex vertex3) { Vertices[0] = vertex1; Vertices[1] = vertex2; Vertices[2] = vertex3; }

	Vertex Vertices[3];
};

struct Group
{
	string			name;
	Material*		material;

	vector<Triangle>*	triangles;

	Group() { triangles = new vector<Triangle>(); }
};


#endif

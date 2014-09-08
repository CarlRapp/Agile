#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include <vector>

#include "../stdafx.h"
#include "Vertex.h"

class ModelData
{
private:
	std::vector<Vertex> m_vertices;

public:
	ModelData(std::vector<Vertex> vertices);
	~ModelData();

	Vertex GetVertex(int id) { return m_vertices[id]; }
	std::vector<Vertex> GetVertices() { return m_vertices; }
};


#endif
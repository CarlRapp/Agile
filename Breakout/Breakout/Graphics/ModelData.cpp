#include "ModelData.h"

ModelData::ModelData(std::vector<Vertex> vertices)
{
	m_vertices = vertices;
}

ModelData::~ModelData()
{
	m_vertices.erase(m_vertices.begin(), m_vertices.end());
}

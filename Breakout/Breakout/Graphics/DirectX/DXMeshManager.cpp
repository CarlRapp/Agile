#include "DXMeshManager.h"

DXMeshManager::DXMeshManager(void)
{
	m_meshes = map<string, DXMesh*>();
}

DXMeshManager::~DXMeshManager()
{
	map <string, DXMesh*>::iterator	meshIterator;
	for (meshIterator = m_meshes.begin(); meshIterator != m_meshes.end(); ++meshIterator)
		delete	meshIterator->second;

	m_meshes.clear();
}


DXMesh* DXMeshManager::GetMesh(string _name)
{
	if (m_meshes.count(_name) != 0)
		return m_meshes[_name];

	return 0;
}

void DXMeshManager::AddMesh(string _name, DXMesh* _mesh)
{
	if (m_meshes.count(_name) == 0)
	{
		m_meshes[_name] = _mesh;
	}
}

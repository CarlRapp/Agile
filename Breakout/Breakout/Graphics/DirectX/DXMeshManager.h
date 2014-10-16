#pragma once
#ifndef DXMESHMANAGER_H
#define DXMESHMANAGER_H

#include "DXMesh.h"
#include <map>
using namespace std;

class DXMeshManager
{
private:

	std::map<string, DXMesh*>			m_meshes;

public:
	DXMeshManager(void);
	~DXMeshManager(void);


	DXMesh*	GetMesh(string _name);
	void AddMesh(string _name, DXMesh* _mesh);

	//ModelInstance*	CreateModelInstance(string ModelName);
};

#endif
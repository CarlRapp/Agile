#pragma once
#ifndef DXMODELMANAGER_H
#define DXMODELMANAGER_H

#include "DXModel.h"
#include "../../Storage/FileManager.h"
#include "DXTextureManager.h"
#include "DXMeshManager.h"

using namespace std;

class DXModelManager
{
private:

	map <string, DXModel*>			m_loadedModels;

	DXMeshManager m_meshManager;

public:
	DXModelManager(void);
	~DXModelManager(void);


	DXModel*	GetModel(string _path);
	void	LoadModel(ID3D11Device* _device, string _path, DXTextureManager &_texMgr);
	void	LoadModel(ID3D11Device* _device, string _path, string _blendTexture, DXTextureManager &_texMgr);

	//ModelInstance*	CreateModelInstance(string ModelName);
};

#endif
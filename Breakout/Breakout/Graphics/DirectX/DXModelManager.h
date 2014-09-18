#pragma once
#ifndef DXMODELMANAGER_H
#define DXMODELMANAGER_H

#include "DXModel.h"
#include "../../Storage/FileManager.h"
#include "DXTextureManager.h"
using namespace std;

class DXModelManager
{
private:

	DXTextureManager	m_textureManager;

	map <string, DXModel*>			m_loadedModels;
	map <string, DXModel*>::iterator	m_modelIterator;

public:
	DXModelManager(void);
	~DXModelManager(void);


	DXModel*	GetModel(string _path);
	void	LoadModel(ID3D11Device* _device, string _path);

	//ModelInstance*	CreateModelInstance(string ModelName);
};

#endif
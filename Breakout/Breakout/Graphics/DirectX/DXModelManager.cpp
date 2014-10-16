#include "DXModelManager.h"

DXModelManager::DXModelManager(void)
{
	m_loadedModels	=	map<string, DXModel*>();
}

DXModelManager::~DXModelManager()
{
	for (m_modelIterator = m_loadedModels.begin(); m_modelIterator != m_loadedModels.end(); ++m_modelIterator)
		delete	m_modelIterator->second;

	m_loadedModels.clear();
}


void DXModelManager::LoadModel(ID3D11Device* _device, string _path, DXTextureManager &_texMgr)
{
	ModelData* data = FileManager::GetInstance().LoadModel(GetFile(_path, MODEL_ROOT));

	if (m_loadedModels.count(_path) != 0)
		return;

	DXModel*	tModel = new DXModel(_device, _texMgr, data);

	if ( tModel == 0 )
	{
		return;
	}

	m_loadedModels.insert(pair<string, DXModel*>(_path, tModel));
}


DXModel* DXModelManager::GetModel(string _name)
{
	if (m_loadedModels.count(_name) != 0)
		return m_loadedModels[_name];

	return 0;
}

/*
ModelInstance* DXModelManager::CreateModelInstance(string ModelName)
{
	DXModel*	tModel = GetModel(ModelName);

	if ( tModel == 0 )
		return 0;

	ModelInstance*	tInstance	=	new ModelInstance();
	tInstance->SetModel(tModel);
	//tInstance->m_Model	=	tModel;

	return	tInstance;
}
*/
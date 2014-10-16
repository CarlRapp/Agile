#include "DXModelManager.h"

DXModelManager::DXModelManager(void)
{
	m_loadedModels	=	map<string, DXModel*>();
	m_meshManager;
}

DXModelManager::~DXModelManager()
{
	map <string, DXModel*>::iterator	modelIterator;
	for (modelIterator = m_loadedModels.begin(); modelIterator != m_loadedModels.end(); ++modelIterator)
		delete	modelIterator->second;

	m_loadedModels.clear();

	m_meshManager;
}




void DXModelManager::LoadModel(ID3D11Device* _device, string _path, DXTextureManager &_texMgr)
{
	if (m_loadedModels.count(_path) != 0)
		return;

	ModelData* data = FileManager::GetInstance().LoadModel(GetFile(_path, MODEL_ROOT));

	DXModel*	tModel = new DXModel(_device, _texMgr, data, _path, m_meshManager);

	if (tModel == 0)
	{
		return;
	}

	m_loadedModels.insert(pair<string, DXModel*>(_path, tModel));
}

void DXModelManager::LoadModel(ID3D11Device* _device, string _path, string _blendTexture, DXTextureManager &_texMgr)
{
	std::string path = _path + _blendTexture;
	

	if (m_loadedModels.count(path) != 0)
		return;

	if (m_loadedModels.count(_path) == 0)
		LoadModel(_device, _path, _texMgr);

	DXModel* parent = GetModel(_path);

	if (!parent)
		return;

	DXModel* child = new DXModel(parent, _blendTexture);

	child->SetBlendTexture(_blendTexture, _texMgr);

	m_loadedModels.insert(pair<string, DXModel*>(path, child));
}

DXModel* DXModelManager::GetModel(string _name)
{
	if (m_loadedModels.count(_name) != 0)
		return m_loadedModels[_name];

	return 0;
}

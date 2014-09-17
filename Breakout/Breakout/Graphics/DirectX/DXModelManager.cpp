#include "ModelManager.h"

ModelManager* ModelManager::gInstance	=	0;
ModelManager::ModelManager(ID3D11Device* Device)
{
	gLoadedModels	=	map<string, Model*>();
	gDevice			=	Device;

	gTextureManager.Init(Device);
}
ModelManager::~ModelManager()
{
	for ( gModelIterator = gLoadedModels.begin(); gModelIterator != gLoadedModels.end(); ++gModelIterator )
		delete	gModelIterator->second;

	gLoadedModels.clear();
}

bool ModelManager::IsInitialized()
{
	return gInstance != 0;
}

void ModelManager::Initialize(ID3D11Device* Device)
{
	if ( IsInitialized() )
		return;

	gInstance		=	new ModelManager(Device);
}

ModelManager* ModelManager::GetInstance()
{
	return gInstance;
}

void ModelManager::LoadModel(string Name, string Filename, string ModelPath, string TexturePath)
{
	if ( gLoadedModels.count(Name) != 0 )
		return;

	Model*	tModel	=	new Model(gDevice, gTextureManager, ModelPath + Filename, TexturePath);

	if ( tModel == 0 )
	{
		DebugScreen::GetInstance()->AddLogMessage("Model: \"" + Filename + "\" could not be loaded.", Red);
		return;
	}

	gLoadedModels.insert(pair<string, Model*>(Name, tModel));
}

void ModelManager::LoadModel(string Name, string Filename, string Path)
{
	LoadModel(Name, Filename, Path, Path);
}

Model* ModelManager::GetModel(string Name)
{
	if ( gLoadedModels.count(Name) != 0 )
		return gLoadedModels[Name];

	DebugScreen::GetInstance()->AddLogMessage("Model: \"" + Name + "\" is not loaded.", Red);
	return 0;
}

ModelInstance* ModelManager::CreateModelInstance(string ModelName)
{
	Model*	tModel	=	GetModel(ModelName);

	if ( tModel == 0 )
		return 0;

	ModelInstance*	tInstance	=	new ModelInstance();
	tInstance->SetModel(tModel);
	//tInstance->m_Model	=	tModel;

	return	tInstance;
}
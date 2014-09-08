#include "FileManager.h"

FileManager* FileManager::instance = 0;


ModelData* FileManager::LoadModel(std::string filePath)
{
	auto modelIt = m_modelMap.find(filePath);
	if (modelIt == m_modelMap.end())
	{

		//Model* model = new Model();
	}
	else
		return modelIt->second;
	return 0;
}
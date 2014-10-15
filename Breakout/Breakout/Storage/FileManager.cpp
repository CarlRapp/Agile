#include "FileManager.h"

FileManager* FileManager::instance = 0;

FileManager::~FileManager()
{
	for (auto it : m_modelMap)
	{
		for (int i = 0; i < it.second->Groups.size(); ++i)
		{
			SafeDelete(it.second->Groups[i]->material);
			SafeDelete(it.second->Groups[i]->triangles);
			SafeDelete(it.second->Groups[i]);
		}
		SafeDelete(it.second);
		it.second = 0;
	}

	m_modelMap.clear();

	for (auto it : m_mixChunkMap)
	{
		Mix_FreeChunk(it.second);
		it.second = 0;
	}

	for (auto it : m_mixMusicMap)
	{
		Mix_FreeMusic(it.second);
		it.second = 0;
	}

	SafeDelete(m_modelLoader);

}

ModelData* FileManager::LoadModel(std::string filePath)
{
	auto modelIt = m_modelMap.find(filePath);
	if (modelIt == m_modelMap.end())
	{
		ModelData* data = ModelLoader().LoadModelFile(filePath);
		if (data)
			m_modelMap.insert(std::pair<std::string, ModelData*>(filePath, data));
		return data;
	}
	else
		return modelIt->second;
	return 0;
}

Mix_Chunk* FileManager::LoadSoundEffect(std::string filePath)
{
	auto mixChunkIt = m_mixChunkMap.find(filePath);
	if (mixChunkIt == m_mixChunkMap.end())
	{
		Mix_Chunk* data = WavLoader::LoadWavFile(filePath);
		if (data)
			m_mixChunkMap.insert(std::pair<std::string, Mix_Chunk*>(filePath, data));
		return data;
	}
	else
		return mixChunkIt->second;
	return 0;
}

Mix_Music* FileManager::LoadMusic(std::string filePath)
{
	auto mixMusicIt = m_mixMusicMap.find(filePath);
	if (mixMusicIt == m_mixMusicMap.end())
	{
		Mix_Music* data = MusicLoader::LoadMusicFile(filePath);
		if (data)
			m_mixMusicMap.insert(std::pair<std::string, Mix_Music*>(filePath, data));
		return data;
	}
	else
		return mixMusicIt->second;
	return 0;
}
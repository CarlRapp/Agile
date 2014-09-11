#include "FileManager.h"


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
#include "FileManager.h"


ModelData* FileManager::LoadModel(std::string _filePath)
{
        _filePath = GetFile(_filePath, MODEL_ROOT);
	auto modelIt = m_modelMap.find(_filePath);
	if (modelIt == m_modelMap.end())
	{
		ModelData* data = ObjLoader::LoadObjFile(_filePath);
		if (data)
                {
                    m_modelMap.insert(std::pair<std::string, ModelData*>(_filePath, data));
                    printf("Model load success: %s\n",_filePath.c_str());
                }
                else
                    printf("Model load failed: %s\n",_filePath.c_str());
		return data;
	}
	else
		return modelIt->second;
        
        
	return 0;
}

Mix_Chunk* FileManager::LoadSoundEffect(std::string _filePath)
{
	auto mixChunkIt = m_mixChunkMap.find(_filePath);
	if (mixChunkIt == m_mixChunkMap.end())
	{
		Mix_Chunk* data = WavLoader::LoadWavFile(_filePath);
		if (data)
			m_mixChunkMap.insert(std::pair<std::string, Mix_Chunk*>(_filePath, data));
		return data;
	}
	else
		return mixChunkIt->second;
	return 0;
}

Mix_Music* FileManager::LoadMusic(std::string _filePath)
{
	auto mixMusicIt = m_mixMusicMap.find(_filePath);
	if (mixMusicIt == m_mixMusicMap.end())
	{
		Mix_Music* data = MusicLoader::LoadMusicFile(_filePath);
		if (data)
			m_mixMusicMap.insert(std::pair<std::string, Mix_Music*>(_filePath, data));
		return data;
	}
	else
		return mixMusicIt->second;
	return 0;
}
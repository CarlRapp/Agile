#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef LINUX
#include <dirent.h>
#include <unistd.h>
#endif

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
void FileManager::GetFilesInDirectory(std::vector<std::string>& out, const std::string directory)
{
#ifdef WINDOWS
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory.c_str());
        //printf("Trying to open: %s\n", directory.c_str());
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		out.push_back(file_name);
	}
	closedir(dir);
#endif
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

std::vector<HighScore> FileManager::LoadHighScores(std::string filePath)
{
	if (m_highScores.size() == 0)
		return HighScoreLoader::LoadHighScores(filePath);
	else
		return m_highScores;
}

void FileManager::TryAddHighScore(std::string filePath, std::string name, int score)
{
	/// High score list has to be full from the beginning
	if (m_highScores.size() != 5)
	{
		m_highScores = LoadHighScores(filePath);
		/// Make sure that the list is full, even if its with shitty data that won't be saved
		while (m_highScores.size() < 5)
		{
			HighScore highScore;
			highScore.m_name = "";
			highScore.m_score = 0;
			m_highScores.push_back(highScore);
		}
	}
	int newScoreIndex = -1;
	/// Get the position in the list of the new high score if it actually is a high score
	for (int i = 0; i < 5; ++i)
	{
		HighScore& highScore = m_highScores.at(i);
		if (score > highScore.m_score)
		{
			newScoreIndex = i;
			break;
		}
	}
	/// If the new score is actually an high score, then add it to the list and save
	if (newScoreIndex >= 0)
	{
		for (int i = m_highScores.size() - 1; i >= newScoreIndex + 1; --i)
		{
			m_highScores.at(i) = m_highScores.at(i - 1);
		}
		m_highScores.at(newScoreIndex).m_name = name;
		m_highScores.at(newScoreIndex).m_score = score;
		HighScoreLoader::CreateHighScores(filePath, m_highScores);
	}
}
#include "HighScoreLoader.h"

#include <iostream>
#include <fstream>
#include <vector>

std::vector<HighScore> HighScoreLoader::LoadHighScores(std::string _path)
{
	std::ifstream file(_path);
	if (!file)
	{
		printf("ERROR: Cannot open high score file: %s\n", _path);
		return std::vector<HighScore>();
	}
	std::vector<HighScore> lines;
	std::string line;
	while (getline(file, line))
	{
		int space = line.find(' ');
		HighScore highScore;
		highScore.m_name = line.substr(0, space);
		highScore.m_score = stoi(line.substr(space, line.size() - space));
		lines.push_back(highScore);
	}
	file.close();
	return lines;
}

void HighScoreLoader::CreateHighScores(std::string _path, std::vector<HighScore> _data)
{
	std::ofstream file(_path);
	if (!file)
	{
		printf("ERROR: Cannot open high score file: %s\n", _path);
		return;
	}
	for (auto highScore : _data)
	{
		// Name can't be empty
		if (highScore.m_name == "")
			continue;
		std::string text = highScore.m_name + " ";
		text += std::to_string(highScore.m_score);
		file << text << std::endl;
	}

	file.close();
}
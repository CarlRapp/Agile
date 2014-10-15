#ifndef HIGHSCORELOADER_H
#define HIGHSCORELOADER_H

#include <string>
#include <vector>

struct HighScore
{
	int m_score;
	std::string m_name;
};

class HighScoreLoader
{
public:
	static std::vector<HighScore> LoadHighScores(std::string _path);
	static void CreateHighScores(std::string _path, std::vector<HighScore> _data);
};
#endif

#ifndef WINAUDIO_H
#define WINAUDIO_H

#include "IAudio.h"
#include <Windows.h>


enum MESSAGE_TYPE
{
	MSG_NORMAL = 0,
	MSG_WARNING,
	MSG_ERROR
};


#pragma region ConsoleLog
template <typename T>
void LogMessage(T _t)
{
	std::cout << " " << _t << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
template <typename T, typename... Args>
void LogMessage(T _t, Args... _args)
{
	std::cout << " " << t;
	LogMessage(_args);
}

template<typename T>
void Log(MESSAGE_TYPE _type, T _t)
{
	std::string type = "";

	switch (_type)
	{
	case MSG_NORMAL:
		type = "[MESSAGE]";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Green
		break;
	case MSG_WARNING:
		type = "[WARNING]";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow
		break;
	case MSG_ERROR:
		type = "[ERROR]";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Red
		break;
	default:
		break;
	}

	std::cout << type << " " << _t << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

template <typename T, typename... Args>
void Log(MESSAGE_TYPE _type, T _t, Args... _args)
{
	std::string type = "";

	switch (_type)
	{
	case MSG_NORMAL:
		type = "[MESSAGE]";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Green
		break;
	case MSG_WARNING:
		type = "[WARNING]";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow
		break;
	case MSG_ERROR:
		type = "[ERROR]";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Red
		break;
	default:
		break;
	}

	std::cout << type << " " << _t;
	LogMessage(_args...);
}
#pragma endregion

class WinAudio : public IAudio
{

private:

public:
	WinAudio();
	~WinAudio();

	bool Initialize();

	bool PlayMusic(const char* _fileName, int _loop);
	bool PlaySoundEffect(const char* _fileName, int _loop);

	bool LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music);
	bool LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect);

};


#endif
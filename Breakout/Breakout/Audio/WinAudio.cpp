#include "WinAudio.h"

WinAudio::WinAudio() 
: IAudio()
{
}

WinAudio::~WinAudio()
{
	for (std::map<std::string, Mix_Music*>::iterator iter = m_music.begin(); iter != m_music.end(); ++iter)
	{
		Mix_FreeMusic(iter->second);
		iter->second = NULL;
	}

	for (std::map<std::string, Mix_Chunk*>::iterator iter = m_soundEffects.begin(); iter != m_soundEffects.end(); ++iter)
	{
		Mix_FreeChunk(iter->second);
		iter->second = NULL;
	}

	Mix_Quit();
	SDL_Quit();

}

bool WinAudio::Initialize()
{

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		Log(MSG_ERROR, "SDL_Audio:", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Log(MSG_ERROR, "SDL_Audio:", SDL_GetError());
		return false;
	}



	return true;
}

bool WinAudio::LoadMusic(const char* _filePath, const char* _fileName, Mix_Music* _music)
{
	std::map<std::string, Mix_Music*>::iterator iter = m_music.find(_fileName);
	if (iter != m_music.end())
	{
		Log(MSG_WARNING, "SDL_Audio, Music already exist:", _fileName);
		return true;
	}

	Mix_Music* music = NULL;

	music = Mix_LoadMUS(_filePath);
	if (music == NULL)
	{
		Log(MSG_ERROR, "SDL_Audio:", Mix_GetError());
		return false;
	}

	_music = music;

	// Temp for testing
	m_music.insert(std::pair<std::string, Mix_Music*>(_fileName, music));
	Log(MSG_NORMAL, "SDL_Audio, added music file:", _fileName);

	return true;
}

bool WinAudio::LoadSoundEffect(const char* _filePath, const char* _fileName, Mix_Chunk* _soundEffect)
{

	std::map<std::string, Mix_Chunk*>::iterator iter = m_soundEffects.find(_fileName);
	if (iter != m_soundEffects.end())
	{
		Log(MSG_WARNING, "SDL_Audio, SoundEffect already exist:", _fileName);
		return true;
	}

	Mix_Chunk* soundEffect = NULL;

	soundEffect = Mix_LoadWAV(_filePath);
	if (soundEffect == NULL)
	{
		Log(MSG_ERROR, "SDL_Audio:", Mix_GetError());
		return false;
	}

	_soundEffect = soundEffect;

	// Temp for testing
	m_soundEffects.insert(std::pair<std::string, Mix_Chunk*>(_fileName, soundEffect));
	Log(MSG_NORMAL, "SDL_Audio, added sound effect:", _fileName);

	return true;
}

bool WinAudio::PlayMusic(const char* _fileName, int _loop)
{
	// Music not found, return false
	std::map<std::string, Mix_Music*>::iterator iter = m_music.find(_fileName);
	if (iter == m_music.end())
	{
		Log(MSG_ERROR, "SDL_Audio, Music not found:", _fileName);
		return false;
	}

	// Another music already playing, switching
	if (Mix_PlayingMusic() != 0)
		Log(MSG_WARNING, "SDL_Audio: Already playing track. Changing to new."); // Could possible be removed

	// Play the music track
	if(Mix_PlayMusic(iter->second, _loop) == -1)
	{
		Log(MSG_ERROR, "SDL_Audio:", Mix_GetError());
		return false;
	}

	return true;
}

bool WinAudio::PlaySoundEffect(const char* _fileName, int _loop)
{
	// Sound not found, return false
	std::map<std::string, Mix_Chunk*>::iterator iter = m_soundEffects.find(_fileName);
	if (iter == m_soundEffects.end())
	{
		Log(MSG_ERROR, "SDL_Audio, Music not found:", _fileName);
		return false;
	}

	// Play the sound
	if (Mix_PlayChannel(-1, iter->second, _loop) == -1)
	{
		Log(MSG_ERROR, "SDL_Audio:", Mix_GetError());
		return false;
	}

	return true;
}
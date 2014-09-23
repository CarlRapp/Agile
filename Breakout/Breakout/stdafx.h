#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <string>

#ifdef __linux__
#define LINUX
#define OPENGL
#include <unistd.h>

#define MODEL_ROOT "/../../Data/Models/"
#define AUDIO_ROOT "/../../Data/Audio/"

#else
#define WINDOWS
#define DIRECTX

#define MODEL_ROOT "../../Data/Models/"
#define AUDIO_ROOT "../../Data/Audio/"

#include <Windows.h>
#include "MathHelper.h"

#endif

#define SHADER_ROOT "/Graphics/OpenGL/GLShaders/"
static char    m_cwd[FILENAME_MAX];

#define PI (3.14159265358979323846f)
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = 0; }

#define MAX_ENTITY_COUNT 500

typedef unsigned int        UINT;


static std::string GetFile(std::string _path, std::string _root)
{
#ifdef WINDOWS

	return _root + _path;

#else



	std::string temp = _root;

	if (!getcwd(m_cwd, sizeof(m_cwd)))
	{
		printf("Unable to find current working directory!\n");
		return "";
	}

	//	Open the file

	std::string	workingDir = m_cwd;

	workingDir = workingDir.append(temp);
	_path = workingDir.append(_path);
	return _path;

#endif
}







#endif
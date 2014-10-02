#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <string>

#ifdef __linux__
#define LINUX
#define OPENGL
#include <unistd.h>

#define MODEL_ROOT "/../../Data/Models/"
#define AUDIO_ROOT "/../../Data/Audio/"
#define TEXTURE_ROOT "/../../Data/Textures/"

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;

template <size_t S>
struct _ENUM_FLAG_INTEGER_FOR_SIZE;

template <>
struct _ENUM_FLAG_INTEGER_FOR_SIZE<1>
{
	typedef INT8 type;
};

template <>
struct _ENUM_FLAG_INTEGER_FOR_SIZE<2>
{
	typedef INT16 type;
};

template <>
struct _ENUM_FLAG_INTEGER_FOR_SIZE<4>
{
	typedef INT32 type;
};

// used as an approximation of std::underlying_type<T>
template <class T>
struct _ENUM_FLAG_SIZED_INTEGER
{
	typedef typename _ENUM_FLAG_INTEGER_FOR_SIZE<sizeof(T)>::type type;
};



template <class T>
struct _ENUM_FLAG_SIZED_INTEGER
{
	typedef typename _ENUM_FLAG_INTEGER_FOR_SIZE<sizeof(T)>::type type;
};



#else
#define WINDOWS
#define DIRECTX

#define MODEL_ROOT "../../Data/Models/"
#define AUDIO_ROOT "../../Data/Audio/"
#define TEXTURE_ROOT "../../Data/Textures/"

#include <Windows.h>


#endif
#include "MathHelper.h"
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


//Returns the memory adress as INT
//of the given pointer
static int GetMemoryID(void* _pointer)
{
	return (int)_pointer;
}
#endif
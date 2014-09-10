#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <string>
#include <unistd.h>

#ifdef __linux__
#define LINUX
#define OPENGL
#else
#define WINDOWS
#define DIRECTX
#endif

#define SHADER_ROOT "/Graphics/OpenGL/GLShaders/"
#define MODEL_ROOT "/../../Data/Models/"
static char    m_cwd[FILENAME_MAX];

struct Vector2;
struct Vector3;

static std::string GetFile(std::string _path, std::string _root)
{
    std::string temp = _root;
    
    if (!getcwd(m_cwd, sizeof(m_cwd)))
        {
            printf ("Unable to find current working directory!\n");
            return "";
        }

     //	Open the file

    std::string	workingDir = m_cwd;

    workingDir = workingDir.append(temp);
    _path = workingDir.append(_path);
    return _path;
}

struct Vector2
{
public:
	float X, Y;

	Vector2(float x, float y) : X(x), Y(y) {}
};

struct Vector3
{
public:
	float X, Y, Z;

	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
	Vector3(Vector2 vector, float z) : X(vector.X), Y(vector.Y), Z(z) {}

	Vector2 ToVector2() { return Vector2(X, Y); }
};

#endif
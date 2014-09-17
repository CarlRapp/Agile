#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <string>

#ifdef __linux__
#define LINUX
#define OPENGL
#include <unistd.h>
#else
#define WINDOWS
#define DIRECTX

#include <Windows.h>

#endif

#define SHADER_ROOT "/Graphics/OpenGL/GLShaders/"

#ifdef LINUX
#define MODEL_ROOT "/../../Data/Models/"
#define AUDIO_ROOT "/../../Data/Audio/"
#else
#define MODEL_ROOT "../../Data/Models/"
#define AUDIO_ROOT "../../Data/Audio/"
#endif

static char    m_cwd[FILENAME_MAX];

#define PI (3.14159265358979323846f)
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = 0; }


typedef unsigned int        UINT;

struct Vector2;
struct Vector3;



static std::string GetFile(std::string _path, std::string _root)
{
#ifdef WINDOWS

	return _root + _path;

#else



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

#endif
}




#ifdef LINUX

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MATRIX4 glm::mat4
#define VECTOR3 glm::vec3

    static glm::mat4 MacroTranslate(glm::mat4 matrix,glm::vec3 vector,int dx1,int dx2)
    {
        return glm::translate(matrix,vector);
    }

    static glm::mat4 MacroRotate(glm::mat4 matrix, float angle, glm::vec3 axis)
    {
        return glm::rotate(matrix,angle,axis);
    }

#else

#include <DirectXMath.h>

#define MATRIX4 DirectX::XMFLOAT4X4
#define VECTOR4 DirectX::XMFLOAT4
#define VECTOR3 DirectX::XMFLOAT3
#define VECTOR2 DirectX::XMFLOAT2

static DirectX::XMFLOAT4X4 MacroTranslate(MATRIX4 _mat, float _x, float _y, float _z)
{
	DirectX::XMMATRIX temp;
	temp = DirectX::XMMatrixTranslation(_x, _y, _z);
	DirectX::XMStoreFloat4x4(&_mat, temp);
	return _mat;
}


static DirectX::XMFLOAT4X4 MacroRotate(MATRIX4 _mat, float _angle, VECTOR3 _axis)
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&_axis);
	DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&_mat);
	temp = DirectX::XMMatrixRotationAxis(vec, _angle);
	DirectX::XMStoreFloat4x4(&_mat, temp);
	return _mat;
}


#endif

#define TRANSLATE(matrix,vector,dx1,dx2) MacroTranslate(matrix,vector,dx1,dx2)
#define ROTATE(matrix,angle,axis) MacroRotate(matrix,angle,axis)



#endif

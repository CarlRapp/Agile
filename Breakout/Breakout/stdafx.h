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
#define MODEL_ROOT "Data/Models/"
#define AUDIO_ROOT "Data/Audio/"
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

struct Vector2
{
public:
	float x, y;

	Vector2(float _x, float _y) : x(_x), y(_y) {}
};

struct Vector3
{
public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(Vector2 _vector, float _z) : x(_vector.x), y(_vector.y), z(_z) {}

	Vector2 ToVector2() { return Vector2(x, y); }
};

struct Vector4
{
public:
	float x, y, z, w;

	Vector4() : x(0), y(0), z(0), w(0) {};
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

};

struct Float4x4
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	Float4x4() {}
	Float4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) :
		_11(m00), _12(m01), _13(m02), _14(m03),
		_21(m10), _22(m11), _23(m12), _24(m13),
		_31(m20), _32(m21), _33(m22), _34(m23),
		_41(m30), _42(m31), _43(m32), _44(m33){}
	//explicit Float4x4(_In_reads_(16) const float *pArray);

	float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
	float&      operator() (size_t Row, size_t Column) { return m[Row][Column]; }

	Float4x4& operator= (const Float4x4& Float4x4)
	{
		_11 = Float4x4._11; _12 = Float4x4._12; _13 = Float4x4._13; _14 = Float4x4._14;
		_21 = Float4x4._21; _22 = Float4x4._22; _23 = Float4x4._23; _24 = Float4x4._24;
		_31 = Float4x4._31; _32 = Float4x4._32; _33 = Float4x4._33; _34 = Float4x4._34;
		_41 = Float4x4._41; _42 = Float4x4._42; _43 = Float4x4._43; _44 = Float4x4._44;
		return *this;
	}

};

#endif
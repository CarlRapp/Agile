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
#define MODEL_ROOT "/../../Data/Models/"
#define AUDIO_ROOT "/../../Data/Audio/"
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

	Vector2() : x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	/* Arithmetic */
	Vector2 Vector2::operator+(const Vector2 _vector)	{ return Vector2(x + _vector.x, y + _vector.y); }
	Vector2 Vector2::operator+(const int _value)		{ return Vector2(x + _value, y + _value); }
	Vector2 Vector2::operator+(const float _value)		{ return Vector2(x + _value, y + _value); }
	Vector2 Vector2::operator-(const Vector2 _vector)	{ return Vector2(x - _vector.x, y - _vector.y); }
	Vector2 Vector2::operator-(const int _value)		{ return Vector2(x - _value, y - _value); }
	Vector2 Vector2::operator-(const float _value)		{ return Vector2(x - _value, y - _value); }
	Vector2 Vector2::operator*(const Vector2 _vector)	{ return Vector2(x * _vector.x, y * _vector.y); }
	Vector2 Vector2::operator*(const int _value)		{ return Vector2(x * _value, y * _value); }
	Vector2 Vector2::operator*(const float _value)		{ return Vector2(x * _value, y * _value); }
	Vector2 Vector2::operator/(const Vector2 _vector)	{ return Vector2(x / _vector.x, y / _vector.y); }
	Vector2 Vector2::operator/(const int _value)		{ return Vector2(x / _value, y / _value); }
	Vector2 Vector2::operator/(const float _value)		{ return Vector2(x / _value, y / _value); }
	Vector2 Vector2::operator%(const Vector2 _vector)	{ return Vector2(fmod(x, _vector.x), fmod(y, _vector.y)); }
	Vector2 Vector2::operator%(const int _value)		{ return Vector2(fmod(x, _value), fmod(y, _value)); }
	Vector2 Vector2::operator%(const float _value)		{ return Vector2(fmod(x, _value), fmod(y, _value)); }
	Vector2 Vector2::operator-()						{ return Vector2(-x, -y); }

	/* Comparison */
	bool Vector2::operator==(Vector2 _vector) { return (x == _vector.x && y == _vector.y); }
	bool Vector2::operator!=(Vector2 _vector) { return !(x == _vector.x && y == _vector.y); }

	// Expensive operation, don't do often please!
	Vector2 Normalize() { float magn = sqrtf(x * x + y * y); return Vector2(x / magn, y / magn); }

	inline float Distance(const Vector2 _vector) { float dx = x - _vector.x; float dy = y - _vector.y; return sqrtf(dx * dx + dy * dy); }
};

struct Vector3
{
public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(Vector2 _vector, float _z) : x(_vector.x), y(_vector.y), z(_z) {}

	/* Arithmetic */
	Vector3 Vector3::operator+(const Vector3 _vector)	{ return Vector3(x + _vector.x, y + _vector.y, z + _vector.z); }
	Vector3 Vector3::operator+(const int _value)		{ return Vector3(x + _value, y + _value, z + _value); }
	Vector3 Vector3::operator+(const float _value)		{ return Vector3(x + _value, y + _value, z + _value); }
	Vector3 Vector3::operator-(const Vector3 _vector)	{ return Vector3(x - _vector.x, y - _vector.y, z - _vector.z); }
	Vector3 Vector3::operator-(const int _value)		{ return Vector3(x - _value, y - _value, z - _value); }
	Vector3 Vector3::operator-(const float _value)		{ return Vector3(x - _value, y - _value, z - _value); }
	Vector3 Vector3::operator*(const Vector3 _vector)	{ return Vector3(x * _vector.x, y * _vector.y, z * _vector.z); }
	Vector3 Vector3::operator*(const int _value)		{ return Vector3(x * _value, y * _value, z * _value); }
	Vector3 Vector3::operator*(const float _value)		{ return Vector3(x * _value, y * _value, z * _value); }
	Vector3 Vector3::operator/(const Vector3 _vector)	{ return Vector3(x / _vector.x, y / _vector.y, z / _vector.z); }
	Vector3 Vector3::operator/(const int _value)		{ return Vector3(x / _value, y / _value, z / _value); }
	Vector3 Vector3::operator/(const float _value)		{ return Vector3(x / _value, y / _value, z / _value); }
	Vector3 Vector3::operator%(const Vector3 _vector)	{ return Vector3(fmod(x, _vector.x), fmod(y, _vector.y), fmod(z, _vector.z)); }
	Vector3 Vector3::operator%(const int _value)		{ return Vector3(fmod(x, _value), fmod(y, _value), fmod(z, _value)); }
	Vector3 Vector3::operator%(const float _value)		{ return Vector3(fmod(x, _value), fmod(y, _value), fmod(z, _value)); }
	Vector3 Vector3::operator-()						{ return Vector3(-x, -y, -z); }

	/* Comparison */
	bool Vector3::operator==(const Vector3 _vector)		{ return (x == _vector.x && y == _vector.y && z == _vector.z); }
	bool Vector3::operator!=(const Vector3 _vector)		{ return !(x == _vector.x && y == _vector.y && z == _vector.z); }

	Vector2 ToVector2() { return Vector2(x, y); }

	// Expensive operation, don't do often please!
	Vector3 Normalize() { float magn = sqrtf(x * x + y * y + z * z); return Vector3(x / magn, y / magn, z / magn); }

	inline float Distance(const Vector3 _vector) { float dx = x - _vector.x; float dy = y - _vector.y; float dz = z - _vector.z; return sqrtf(dx * dx + dy * dy + dz * dz); }
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
		float m30, float m31, float m32, float m33);
	//explicit Float4x4(_In_reads_(16) const float *pArray);

	float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
	float&      operator() (size_t Row, size_t Column) { return m[Row][Column]; }

	Float4x4& operator= (const Float4x4& Float4x4);

};

struct BoundingBox2
{
	float x, y, width, height;

	BoundingBox2() : x(0), y(0), width(0), height(0) {}
	BoundingBox2(float _x, float _y, float _width, float _height) : x(_x), y(_y), width(_width), height(_height) {}
};

struct Sphere2
{
	float x, y, radius;

	Sphere2() : x(0), y(0), radius(0) {}
	Sphere2(float _x, float _y, float _radius) : x(_x), y(_y), radius(_radius) {}
};

#endif
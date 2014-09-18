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

#endif

#define SHADER_ROOT "/Graphics/OpenGL/GLShaders/"
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
	Vector2 Vector2::operator+(const Vector2& _vector) const		{ return Vector2(x + _vector.x, y + _vector.y); }
	Vector2 Vector2::operator+(const int& _value) const			{ return Vector2(x + _value, y + _value); }
	Vector2 Vector2::operator+(const float& _value) const		{ return Vector2(x + _value, y + _value); }
	Vector2 Vector2::operator-(const Vector2& _vector) const		{ return Vector2(x - _vector.x, y - _vector.y); }
	Vector2 Vector2::operator-(const int& _value) const			{ return Vector2(x - _value, y - _value); }
	Vector2 Vector2::operator-(const float& _value) const		{ return Vector2(x - _value, y - _value); }
	Vector2 Vector2::operator*(const Vector2& _vector) const		{ return Vector2(x * _vector.x, y * _vector.y); }
	Vector2 Vector2::operator*(const int& _value) const			{ return Vector2(x * _value, y * _value); }
	Vector2 Vector2::operator*(const float& _value) const		{ return Vector2(x * _value, y * _value); }
	Vector2 Vector2::operator/(const Vector2& _vector) const		{ return Vector2(x / _vector.x, y / _vector.y); }
	Vector2 Vector2::operator/(const int& _value) const			{ return Vector2(x / _value, y / _value); }
	Vector2 Vector2::operator/(const float& _value) const		{ return Vector2(x / _value, y / _value); }
	Vector2 Vector2::operator%(const Vector2& _vector) const		{ return Vector2(fmod(x, _vector.x), fmod(y, _vector.y)); }
	Vector2 Vector2::operator%(const int& _value) const			{ return Vector2(fmod(x, _value), fmod(y, _value)); }
	Vector2 Vector2::operator%(const float& _value) const		{ return Vector2(fmod(x, _value), fmod(y, _value)); }
	Vector2 Vector2::operator-() const							{ return Vector2(-x, -y); }

	/* Comparison */
	bool Vector2::operator==(const Vector2& _vector) const		{ return (x == _vector.x && y == _vector.y); }
	bool Vector2::operator!=(const Vector2& _vector) const		{ return !(x == _vector.x && y == _vector.y); }

	// Expensive operation, don't do often please!
	inline Vector2 Normalize() const { float magn = sqrtf(x * x + y * y); return Vector2(x / magn, y / magn); }

	inline float Distance(const Vector2& _vector) const { float dx = x - _vector.x; float dy = y - _vector.y; return sqrtf(dx * dx + dy * dy); }
};

struct Vector3
{
public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {};
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vector3(Vector2 _vector, float _z) : x(_vector.x), y(_vector.y), z(_z) {}

	Vector3& Vector3::operator=(Vector3 v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;

		return *this;
	}

	Vector3& Vector3::operator+=(Vector3 v)
	{
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;

		return *this;
	}

	Vector3 Vector3::operator+(Vector3 v)
	{
		Vector3 vec3;

		vec3.x = this->x + v.x;
		vec3.y = this->y + v.y;
		vec3.z = this->z + v.z;

		return vec3;
	}

	Vector3 Vector3::operator*(int _i)
	{
		Vector3 vec3;

		vec3.x = this->x * _i;
		vec3.y = this->y * _i;
		vec3.z = this->z * _i;

		return vec3;
	}

	Vector3 Vector3::operator*(float _f)
	{
		Vector3 vec3;

		vec3.x = this->x * _f;
		vec3.y = this->y * _f;
		vec3.z = this->z * _f;

		return vec3;
	}

	Vector3 Vector3::operator*(double _d)
	{
		Vector3 vec3;

		vec3.x = this->x * _d;
		vec3.y = this->y * _d;
		vec3.z = this->z * _d;

		return vec3;
	}

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

struct BoundingBox
{
	float x, y, z, width, height, depth;

	BoundingBox() : x(0), y(0), z(0), width(0), height(0), depth(0) {}
	BoundingBox(const float _x, const float _y, const float _z, const float _width, const float _height, const float _depth) : x(_x), y(_y), width(_width), height(_height), depth(_depth) {}
};

struct BoundingSphere
{
	float x, y, z, radius;

	BoundingSphere() : x(0), y(0), z(0), radius(0) {}
	BoundingSphere(const float _x, const float _y, const float _z, const float _radius) : x(_x), y(_y), z(_z), radius(_radius) {}
};



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
#define VECTOR3 DirectX::XMFLOAT3

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

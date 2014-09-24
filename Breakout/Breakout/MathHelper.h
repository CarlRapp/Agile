#ifndef _MATHHELPER_H_
#define _MATHHELPER_H_



#ifdef __linux__
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MATRIX4 glm::mat4
#define VECTOR4 glm::vec4
#define VECTOR3 glm::vec3
#define VECTOR2 glm::vec2

static glm::mat4 GetIdentityMatrix()
{
    return glm::mat4();
}

static glm::mat4 MacroTranslate(glm::vec3 vector)
{
    
	return glm::translate(glm::mat4(1.0f), vector);
}

static glm::mat4 MacroRotate(glm::mat4 matrix, float angle, glm::vec3 axis)
{
	return glm::rotate(matrix, angle, axis);
}

static bool MacroIsZero(glm::vec3 _vec)
{
	if (_vec.x == 0 && _vec.y == 0 && _vec.z == 0)
		return true;

	return false;
}

static float MacroDot(glm::vec3, glm::vec3)
{
	//FIXA!
	return 1.0f;
}

static glm::vec3 MacroCross(glm::vec3 vector1, glm::vec3 vector2)
{
	//FIXA!
	VECTOR3 a;
	return a;
}

static glm::vec3 MacroNormalize(glm::vec3 vector)
{
	//FIXA!
	VECTOR3 a;
	return a;
}

#else

#include <DirectXMath.h>

#define MATRIX4 DirectX::XMFLOAT4X4
#define VECTOR4 DirectX::XMFLOAT4
#define VECTOR3 DirectX::XMFLOAT3
#define VECTOR2 DirectX::XMFLOAT2

static DirectX::XMFLOAT4X4 MacroTranslate()
{
    XMFLOAT4X4 temp;
    DirectX::XMStoreFloat4x4(&temp,DirectX::XMMatrixIdentity());
    return temp;
}

static DirectX::XMFLOAT4X4 GetIdentityMatrix(VECTOR3 _vector)
{
	DirectX::XMMATRIX temp;
	temp = DirectX::XMMatrixTranslation(_vector.x, _vector.y, _vector.z);

	DirectX::XMFLOAT4X4 float4x4;
	DirectX::XMStoreFloat4x4(&float4x4, temp);

	return float4x4;
}


static DirectX::XMFLOAT4X4 MacroRotate(MATRIX4 _mat, float _angle, VECTOR3 _axis)
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&_axis);
	DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&_mat);
	temp = DirectX::XMMatrixRotationAxis(vec, _angle);
	DirectX::XMStoreFloat4x4(&_mat, temp);
	return _mat;
}

static bool MacroIsZero(VECTOR3 _vec)
{
	if (_vec.x == 0 && _vec.y == 0 && _vec.z == 0)
		return true;

	return false;
}

static float MacroDot(VECTOR3 vector1, VECTOR3 vector2)
{
	DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vector1);
	DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vector2);
	float dot;
	DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(v1, v2));
	return dot;
}

static VECTOR3 MacroCross(VECTOR3 vector1, VECTOR3 vector2)
{
	DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vector1);
	DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vector2);
	VECTOR3 cross;
	DirectX::XMStoreFloat3(&cross, DirectX::XMVector3Cross(v1, v2));
	return cross;
}

static VECTOR3 MacroNormalize(VECTOR3 vector)
{
	DirectX::XMVECTOR lvec(DirectX::XMLoadFloat3(&vector));

	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVector3Normalize(lvec));
	return res;
}


DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r);

DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r);

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r);

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 l, float r);



#endif

#define GETIMATRIX() GetIdentityMatrix()

#define TRANSLATE(vector) MacroTranslate(vector)
#define ROTATE(matrix,angle,axis) MacroRotate(matrix,angle,axis)
#define ISZERO(vector) MacroIsZero(vector)

#define DOT(vector1, vector2) MacroDot(vector1, vector2)
#define CROSS(vector1, vector2) MacroCross(vector1, vector2)
#define NORMALIZE(vector) MacroNormalize(vector)

#endif


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

static DirectX::XMFLOAT4X4 GetIdentityMatrix()
{
	DirectX::XMFLOAT4X4 temp;
    DirectX::XMStoreFloat4x4(&temp,DirectX::XMMatrixIdentity());
    return temp;
}

static DirectX::XMFLOAT4X4 MacroTranslate(VECTOR3 _vector)
{
	DirectX::XMMATRIX temp;
	temp = DirectX::XMMatrixTranslation(_vector.x, _vector.y, _vector.z);

	DirectX::XMFLOAT4X4 float4x4;
	DirectX::XMStoreFloat4x4(&float4x4, temp);

	return float4x4;
}

static DirectX::XMFLOAT4 MacroRotateAxis(VECTOR3 _axis, float angle)
{
	DirectX::XMVECTOR axis = DirectX::XMLoadFloat3(&_axis);

	// If Axis is a normalized vector, it is faster to use the XMMatrixRotationNormal function to build this type of matrix.
	DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationAxis(axis, angle);

	DirectX::XMFLOAT4 q;
	DirectX::XMStoreFloat4(&q, quat);
	return q;
}


static DirectX::XMFLOAT4 MacroRotateYawPitchRoll(float yaw, float pitch, float roll)
{
	// If Axis is a normalized vector, it is faster to use the XMMatrixRotationNormal function to build this type of matrix.
	DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(yaw, pitch, roll);

	DirectX::XMFLOAT4 q;
	DirectX::XMStoreFloat4(&q, quat);
	return q;
}

static DirectX::XMFLOAT4 MacroRotateYawPitchRollFromVector(VECTOR3 _rotation)
{
	// If Axis is a normalized vector, it is faster to use the XMMatrixRotationNormal function to build this type of matrix.
	DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&_rotation);
	DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYawFromVector(rot);

	DirectX::XMFLOAT4 q;
	DirectX::XMStoreFloat4(&q, quat);
	return q;
}

static DirectX::XMFLOAT4X4 MacroRotate(VECTOR4 _quat)
{
	DirectX::XMVECTOR quat = DirectX::XMLoadFloat4(&_quat);
	DirectX::XMMATRIX temp;

	// If Axis is a normalized vector, it is faster to use the XMMatrixRotationNormal function to build this type of matrix.
	temp = DirectX::XMMatrixRotationQuaternion(quat);

	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, temp);
	return mat;
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
DirectX::XMFLOAT4X4 operator*(DirectX::XMFLOAT4X4 _inM1, DirectX::XMFLOAT4X4 _inM2);



#endif

#define GETIMATRIX() GetIdentityMatrix()

#define TRANSLATE(vector) MacroTranslate(vector)
#define ROTATE(quat) MacroRotate(quat)

#define ROTATEAXIS(axis, angle) MacroRotateAxis(axis, angle)
#define ROTATEYAWPITCHROLL(yaw, pitch, roll) MacroRotateYawPitchRoll(yaw, pitch, roll)
#define ROTATEYAWPITCHROLLFROMVECTOR(rot) MacroRotateYawPitchRollFromVector(rot)

#define ISZERO(vector) MacroIsZero(vector)

#define DOT(vector1, vector2) MacroDot(vector1, vector2)
#define CROSS(vector1, vector2) MacroCross(vector1, vector2)
#define NORMALIZE(vector) MacroNormalize(vector)

#endif


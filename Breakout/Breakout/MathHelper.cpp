#include "MathHelper.h"
#include "stdafx.h"

#ifdef WINDOWS
DirectX::XMFLOAT3 operator-(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r)
{
	DirectX::XMVECTOR lvec(DirectX::XMLoadFloat3(&l));
	DirectX::XMVECTOR rvec(DirectX::XMLoadFloat3(&r));

	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVectorSubtract(lvec, rvec));
	return res;
}

DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r)
{
	DirectX::XMVECTOR lvec(DirectX::XMLoadFloat3(&l));
	DirectX::XMVECTOR rvec(DirectX::XMLoadFloat3(&r));

	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVectorAdd(lvec, rvec));
	return res;
}

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r)
{
	DirectX::XMVECTOR lvec(DirectX::XMLoadFloat3(&l));
	DirectX::XMVECTOR rvec(DirectX::XMLoadFloat3(&r));

	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVectorMultiply(lvec, rvec));
	return res;
}

DirectX::XMFLOAT3 operator*(DirectX::XMFLOAT3 l, float r)
{
	DirectX::XMVECTOR lvec(DirectX::XMLoadFloat3(&l));
	DirectX::XMVECTOR rvec(DirectX::XMLoadFloat(&r));

	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVectorMultiply(lvec, rvec));
	return res;
}

DirectX::XMFLOAT4X4 operator*(DirectX::XMFLOAT4X4 _inM1, DirectX::XMFLOAT4X4 _inM2)
{
	DirectX::XMMATRIX m1(DirectX::XMLoadFloat4x4(&_inM1));
	DirectX::XMMATRIX m2(DirectX::XMLoadFloat4x4(&_inM2));

	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixMultiply(m1, m2));
	return result;
}

#endif
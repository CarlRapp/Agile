#include "MathHelper.h"


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
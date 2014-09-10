#include "DXCamera.h"

DXCamera::DXCamera(void)
{
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	m_Fovy = (float)59 * PI * 1.0f / 180.0f;
	m_AspectRatio = 16.0f / 9.0f;
	m_NearZ = 1.0f;
	m_FarZ = 420.0f;

	m_Position = Vector3(0, 0, 0);
	m_Forward = Vector3(0, 0, 1);
	m_Right = Vector3(1, 0, 0);
	m_Up = Vector3(0, 1, 0);

	UpdateView();
	UpdateProjection();
}

DXCamera::DXCamera(float fovy, float aspectRatio, float nearZ, float farZ)
{
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;


	m_Fovy = 2 * atan(tan(0.5f * fovy) * aspectRatio);
	m_AspectRatio = aspectRatio;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	m_Position = Vector3(0, 0, 0);
	m_Forward = Vector3(0, 0, 1);
	m_Right = Vector3(1, 0, 0);
	m_Up = Vector3(0, 1, 0);

	UpdateView();
	UpdateProjection();
}

DXCamera::~DXCamera(void)
{

}


void DXCamera::UpdateView()
{
	DirectX::XMFLOAT3 up, right, forward, position;
	
	memcpy(&up, &m_Up, sizeof(DirectX::XMFLOAT3));
	memcpy(&right, &m_Right, sizeof(DirectX::XMFLOAT3));
	memcpy(&forward, &m_Forward, sizeof(DirectX::XMFLOAT3));
	memcpy(&position, &m_Position, sizeof(DirectX::XMFLOAT3));

	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&right);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&up);
	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&forward);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = DirectX::XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	DirectX::XMStoreFloat3(&right, R);
	DirectX::XMStoreFloat3(&up, U);
	DirectX::XMStoreFloat3(&forward, L);

	m_View(0, 0) = right.x;
	m_View(1, 0) = right.y;
	m_View(2, 0) = right.z;
	m_View(3, 0) = x;

	m_View(0, 1) = up.x;
	m_View(1, 1) = up.y;
	m_View(2, 1) = up.z;
	m_View(3, 1) = y;

	m_View(0, 2) = forward.x;
	m_View(1, 2) = forward.y;
	m_View(2, 2) = forward.z;
	m_View(3, 2) = z;

	m_View(0, 3) = 0.0f;
	m_View(1, 3) = 0.0f;
	m_View(2, 3) = 0.0f;
	m_View(3, 3) = 1.0f;
}

void DXCamera::UpdateProjection()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_Fovy, m_AspectRatio, m_NearZ, m_FarZ);
	DirectX::XMFLOAT4X4 proj4x4;
	DirectX::XMStoreFloat4x4(&proj4x4, proj);

	memcpy(&m_Projection, &proj4x4, sizeof(Float4x4));
}

void DXCamera::SetForward(Vector3 forward)
{

	DirectX::XMFLOAT3 up, right, forward2, position;

	memcpy(&up, &m_Up, sizeof(DirectX::XMFLOAT3));
	memcpy(&right, &m_Right, sizeof(DirectX::XMFLOAT3));
	memcpy(&forward2, &forward, sizeof(DirectX::XMFLOAT3));
	memcpy(&position, &m_Position, sizeof(DirectX::XMFLOAT3));

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&forward2);
	direction = DirectX::XMVector3Normalize(direction);
	DirectX::XMVECTOR up2 = DirectX::XMVectorSet(0, 1, 0, 0);

	if (DirectX::XMVector3Equal(direction, DirectX::XMVectorZero()))
		direction = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, -1, 0, 0)))
		up2 = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, 1, 0, 0)))
		up2 = DirectX::XMVectorSet(0, 0, -1, 0);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(pos, direction, up2);

	DirectX::XMFLOAT4X4 view4x4;
	DirectX::XMStoreFloat4x4(&view4x4, view);
	memcpy(&m_View, &view4x4, sizeof(Float4x4));

	m_Right = Vector3(m_View._11, m_View._21, m_View._31);
	m_Up = Vector3(m_View._12, m_View._22, m_View._32);
	m_Forward = Vector3(m_View._13, m_View._23, m_View._33);

	UpdateView();
}

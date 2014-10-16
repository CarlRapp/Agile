#include "DXCamera.h"

DXCamera::DXCamera(void)
{
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_fovy = (float)59 * PI * 1.0f / 180.0f;
	m_aspectRatio = 16.0f / 9.0f;
	m_nearZ = 1.0f;
	m_farZ = 420.0f;

	m_position = DirectX::XMFLOAT3(0, 0, 0);
	m_forward = DirectX::XMFLOAT3(0, 0, 1);
	m_right = DirectX::XMFLOAT3(1, 0, 0);
	m_up = DirectX::XMFLOAT3(0, 1, 0);

	UpdateView();
	UpdateProjection();
}

DXCamera::DXCamera(float _fov, float _width, float _height, float _nearZ, float _farZ)
{
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;


	m_aspectRatio = _width / _height;

	//m_fovy = 2 * atan(tan(0.5f * _fov) * m_aspectRatio);
	m_fovy = (float)_fov * PI * 1.0f / 180.0f;
	m_nearZ = _nearZ;
	m_farZ = _farZ;

	m_position = DirectX::XMFLOAT3(0, 0, 0);
	m_forward = DirectX::XMFLOAT3(0, 0, 1);
	m_right = DirectX::XMFLOAT3(1, 0, 0);
	m_up = DirectX::XMFLOAT3(0, 1, 0);

	UpdateView();
	UpdateProjection();
}

DXCamera::~DXCamera(void)
{

}


void DXCamera::UpdateView()
{
	DirectX::XMFLOAT3 up, right, forward, position;
	
	memcpy(&up, &m_up, sizeof(DirectX::XMFLOAT3));
	memcpy(&right, &m_right, sizeof(DirectX::XMFLOAT3));
	memcpy(&forward, &m_forward, sizeof(DirectX::XMFLOAT3));
	memcpy(&position, &m_position, sizeof(DirectX::XMFLOAT3));

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

	m_view(0, 0) = right.x;
	m_view(1, 0) = right.y;
	m_view(2, 0) = right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = up.x;
	m_view(1, 1) = up.y;
	m_view(2, 1) = up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = forward.x;
	m_view(1, 2) = forward.y;
	m_view(2, 2) = forward.z;
	m_view(3, 2) = z;

	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}

void DXCamera::UpdateProjection()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspectRatio, m_nearZ, m_farZ);
	DirectX::XMStoreFloat4x4(&m_projection, proj);
}

void DXCamera::Move(DirectX::XMFLOAT3 _move)
{
	m_position.x += _move.x;
	m_position.y += _move.y;
	m_position.z -= _move.z;

	UpdateView();
}

void DXCamera::Move(float _move)
{
	m_position.x += (m_forward.x)*_move;
	m_position.y += (m_forward.y)*_move;
	m_position.z += (m_forward.z)*_move;

	UpdateView();
}

void DXCamera::SetForward(DirectX::XMFLOAT3 _forward)
{
	_forward.z *= -1;

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&_forward);
	direction = DirectX::XMVector3Normalize(direction);
	DirectX::XMVECTOR up2 = DirectX::XMVectorSet(0, 1, 0, 0);

	if (DirectX::XMVector3Equal(direction, DirectX::XMVectorZero()))
		direction = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, -1, 0, 0)))
		up2 = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, 1, 0, 0)))
		up2 = DirectX::XMVectorSet(0, 0, -1, 0);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(pos, direction, up2);

	DirectX::XMStoreFloat4x4(&m_view, view);

	m_right = DirectX::XMFLOAT3(m_view._11, m_view._21, m_view._31);
	m_up = DirectX::XMFLOAT3(m_view._12, m_view._22, m_view._32);
	m_forward = DirectX::XMFLOAT3(m_view._13, m_view._23, m_view._33);

	UpdateView();
}

void DXCamera::SetLookAt(DirectX::XMFLOAT3 _target)
{
	DirectX::XMFLOAT3 forward;
	forward.x = _target.x - m_position.x;
	forward.y = _target.y - m_position.y;
	forward.z = -(-_target.z - m_position.z);

	SetForward(forward);
}
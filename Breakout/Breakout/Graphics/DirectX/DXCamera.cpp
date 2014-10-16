#include "DXCamera.h"

DXCamera::DXCamera(void)
{
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_fovy = (float)59 * PI * 1.0f / 180.0f;
	m_aspectRatio = 16.0f / 9.0f;
	m_nearZ = 1.0f;
	m_farZ = 420.0f;

	m_offset = DirectX::XMFLOAT3(0, 0, 0);
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

	//AddShake(0,40, 50, 1, 60);
	//AddShake(1,1, 2, 25, 60);
	
	AddShake(2,0, 0.5f, 60, -5);
	AddShake(0, 40, 50, 1, 60);
}

DXCamera::~DXCamera(void)
{

}


void DXCamera::UpdateView()
{

	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&m_right);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&m_forward);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&GetPosition());

	// Keep camera's axes orthogonal to each other and of unit length.
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = DirectX::XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	DirectX::XMStoreFloat3(&m_right, R);
	DirectX::XMStoreFloat3(&m_up, U);
	DirectX::XMStoreFloat3(&m_forward, L);

	m_view(0, 0) = m_right.x;
	m_view(1, 0) = m_right.y;
	m_view(2, 0) = m_right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = m_up.x;
	m_view(1, 1) = m_up.y;
	m_view(2, 1) = m_up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = m_forward.x;
	m_view(1, 2) = m_forward.y;
	m_view(2, 2) = m_forward.z;
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

	DirectX::XMFLOAT3 pos2 = GetPosition();
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&pos2);
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
	DirectX::XMFLOAT3 pos = GetPosition();

	forward.x = _target.x - pos.x;
	forward.y = _target.y - pos.y;
	forward.z = -(-_target.z - pos.z);

	SetForward(forward);
}

float randf(float _min, float _max)
{
	return _min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_max - _min)));
}

void DXCamera::AddShake(int _id, float _minOffset, float _maxOffset, float _frequency, float _time)
{

	CameraShake cs;
	cs.time = 0.0f;
	cs.loop = _time < 0;
	_time = 50 * _frequency;
	

	ShakeKeyFrame first;
	first.offset = DirectX::XMFLOAT3(0, 0, 0);
	first.startTime = 0;
	
	cs.shakeKeyFrames.push_back(first);

	int numPoints = _frequency * _time;
	float timePerPoint = 1.0f / _frequency;
	for (int i = 1; i < numPoints; ++i)
	{
		ShakeKeyFrame keyFrame;

		keyFrame.offset = NORMALIZE(DirectX::XMFLOAT3(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f)))
			* randf(_minOffset, _maxOffset);

		keyFrame.startTime = i * _time / (float)numPoints;
		cs.shakeKeyFrames.push_back(keyFrame);
	}

	ShakeKeyFrame last;
	last.offset = DirectX::XMFLOAT3(0, 0, 0);
	last.startTime = _time;


	cs.shakeKeyFrames.push_back(last);

	m_cameraShakes[_id] = cs;
}

void DXCamera::RemoveShake(int _id)
{
	m_cameraShakes.erase(_id);
}

void DXCamera::Update(float _dt)
{
	DirectX::XMFLOAT3 newOffset(0, 0, 0);


	for (auto it = m_cameraShakes.begin(); it != m_cameraShakes.end();)
	{
		CameraShake* cs = &it->second;

		cs->time += _dt;

		if (cs->loop && cs->time > cs->shakeKeyFrames[cs->shakeKeyFrames.size() - 1].startTime)
			cs->time = 0;

		for (int j = 1; j < cs->shakeKeyFrames.size(); ++j)
		{
			if (cs->shakeKeyFrames[j].startTime > cs->time)
			{
				float timediff = cs->shakeKeyFrames[j].startTime - cs->shakeKeyFrames[j - 1].startTime;
				float t = cs->time - cs->shakeKeyFrames[j - 1].startTime;

				float lerpFactor = t / timediff;
				DirectX::XMFLOAT3 offset = cs->shakeKeyFrames[j].offset * lerpFactor + cs->shakeKeyFrames[j - 1].offset * (1 - lerpFactor);

				newOffset = newOffset + offset;
				break;
			}
		}

		if (cs->shakeKeyFrames[cs->shakeKeyFrames.size() - 1].startTime < cs->time)
		{
			m_cameraShakes.erase(it++);
		}
		else
			++it;
	}

	if (m_offset != newOffset)
	{
		m_offset = newOffset;
		UpdateView();
	}

}
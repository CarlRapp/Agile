#pragma once
#ifndef DXCAMERA_H
#define DXCAMERA_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "../ICamera.h"
#include <vector>
#include <map>
class DXCamera : public ICamera
{
private:

	struct ShakeKeyFrame
	{
		DirectX::XMFLOAT3 offset;
		float startTime;

		
	};

	struct CameraShake
	{
		float time;
		bool loop;
		std::vector<ShakeKeyFrame> shakeKeyFrames;
	};

	std::map<int, CameraShake> m_cameraShakes;

	D3D11_VIEWPORT m_viewPort;

	float				m_fovy, m_aspectRatio, m_nearZ, m_farZ;
	DirectX::XMFLOAT3				m_position, m_forward, m_right, m_up, m_offset;
	DirectX::XMFLOAT4X4			m_view, m_projection;

	void UpdateView();
	void UpdateProjection();
	void UpdateFrustum();

public:

	DXCamera(void);
	DXCamera(float _fov, float _width, float _height, float _nearZ, float _farZ);
	~DXCamera(void);

	void Update(float _dt);

	void AddShake(int _id, float _minOffset, float _maxOffset, float _frequency, float _time);
	void RemoveShake(int _id);
	//void Rotate(float pitch, float rotY) = 0;

	//void Pitch(float angle) = 0;
	//void RotateY(float angle) = 0;

#pragma region Get Functions
	MATRIX4* GetView()		{ return &m_view; }
	MATRIX4* GetProjection()	{ return &m_projection; }

	DirectX::XMFLOAT3 GetPosition()		{ return m_position + m_offset; }
	DirectX::XMFLOAT3 GetForward()		{ return m_forward; }
	DirectX::XMFLOAT3 GetRight()			{ return m_right; }
	DirectX::XMFLOAT3 GetUp()				{ return m_up; }

	//BoundingFrustum GetFrustum() = 0;

	void* GetViewPort() { return &m_viewPort; }
#pragma endregion

#pragma region Set Functions
	//void SetFOVY(float fovy) = 0;
	//void SetAspectRatio(float aspectRatio) = 0;
	//void SetNearZ(float nearZ) = 0;
	//void SetFarZ(float farZ) = 0;

	void SetPosition(DirectX::XMFLOAT3 _position)			{ m_position = _position; m_position.z *= -1;  UpdateView(); }
	//void SetPosition(float x, float y, float z) = 0;

	void Move(DirectX::XMFLOAT3 _move);
	void Move(float _move);

	void SetForward(DirectX::XMFLOAT3 forward);
	//void SetForward(float x, float y, float z) = 0;

	void SetLookAt(DirectX::XMFLOAT3 _target);

	//void SetLookAt(float x, float y, float z) = 0;

	void SetViewPort(int _x, int _y, int _width, int _height)
	{
		m_viewPort.TopLeftX = (float)_x;
		m_viewPort.TopLeftY = (float)_y;
		m_viewPort.Width = (float)_width;
		m_viewPort.Height = (float)_height;
		m_aspectRatio = (float)_width / (float)_height;
		UpdateProjection();
	}

#pragma endregion



};




#endif
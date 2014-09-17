#pragma once
#ifndef DXCAMERA_H
#define DXCAMERA_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "../ICamera.h"

class DXCamera : public ICamera
{
private:

	D3D11_VIEWPORT m_viewPort;

	float				m_fovy, m_aspectRatio, m_nearZ, m_farZ;
	Vector3				m_position, m_forward, m_right, m_up;
	Float4x4			m_view, m_projection;

	void UpdateView();
	void UpdateProjection();
	void UpdateFrustum();

public:

	DXCamera(void);
	DXCamera(float _fovy, float _aspectRatio, float _nearZ, float _farZ);
	~DXCamera(void);



	//void Rotate(float pitch, float rotY) = 0;

	//void Pitch(float angle) = 0;
	//void RotateY(float angle) = 0;

#pragma region Get Functions
	void* GetView()		{ return &m_view; }
	void* GetProjection()	{ return &m_projection; }

	Vector3 GetPosition()		{ return m_position; }
	Vector3 GetForward()		{ return m_forward; }
	Vector3 GetRight()			{ return m_right; }
	Vector3 GetUp()				{ return m_up; }

	//BoundingFrustum GetFrustum() = 0;

	void* GetViewPort() { return &m_viewPort; }
#pragma endregion

#pragma region Set Functions
	//void SetFOVY(float fovy) = 0;
	//void SetAspectRatio(float aspectRatio) = 0;
	//void SetNearZ(float nearZ) = 0;
	//void SetFarZ(float farZ) = 0;

	void SetPosition(Vector3 _position)			{ m_position = _position; m_position.z *= -1;  UpdateView(); }
	//void SetPosition(float x, float y, float z) = 0;

	void Move(Vector3 _move);
	void Move(float _move);

	void SetForward(Vector3 forward);
	//void SetForward(float x, float y, float z) = 0;

	void SetLookAt(Vector3 _target);

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
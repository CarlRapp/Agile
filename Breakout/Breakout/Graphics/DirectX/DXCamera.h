#pragma once
#ifndef DXCAMERA_H
#define DXCAMERA_H

#include <DirectXMath.h>
#include <d3d11.h>
#include "../ICamera.h"

class DXCamera : public ICamera
{
private:

	D3D11_VIEWPORT m_ViewPort;

	float				m_Fovy, m_AspectRatio, m_NearZ, m_FarZ;
	Vector3				m_Position, m_Forward, m_Right, m_Up;
	Float4x4			m_View, m_Projection;

	void UpdateView();
	void UpdateProjection();
	void UpdateFrustum();

public:

	DXCamera(void);
	DXCamera(float fovy, float aspectRatio, float nearZ, float farZ);
	~DXCamera(void);



	//void Rotate(float pitch, float rotY) = 0;

	//void Pitch(float angle) = 0;
	//void RotateY(float angle) = 0;

#pragma region Get Functions
	Float4x4 GetView()		{ return m_View; }
	Float4x4 GetProjection()	{ return m_Projection; }

	Vector3 GetPosition()		{ return m_Position; }
	Vector3 GetForward()		{ return m_Forward; }
	Vector3 GetRight()			{ return m_Right; }
	Vector3 GetUp()				{ return m_Up; }

	//BoundingFrustum GetFrustum() = 0;

	void* GetViewPort() { return &m_ViewPort; }
#pragma endregion

#pragma region Set Functions
	//void SetFOVY(float fovy) = 0;
	//void SetAspectRatio(float aspectRatio) = 0;
	//void SetNearZ(float nearZ) = 0;
	//void SetFarZ(float farZ) = 0;

	void SetPosition(Vector3 position)			{ m_Position = position; UpdateView(); }
	//void SetPosition(float x, float y, float z) = 0;
	void SetForward(Vector3 forward);
	//void SetForward(float x, float y, float z) = 0;

	//void SetLookAt(Vector3 target) = 0;

	//void SetLookAt(float x, float y, float z) = 0;

	void SetViewPort(int X, int Y, int Width, int Height)
	{
		m_ViewPort.TopLeftX = (float)X;
		m_ViewPort.TopLeftY = (float)Y;
		m_ViewPort.Width = (float)Width;
		m_ViewPort.Height = (float)Height;
	}

#pragma endregion



};




#endif
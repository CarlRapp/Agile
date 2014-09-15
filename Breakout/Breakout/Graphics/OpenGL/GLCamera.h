#pragma once
#ifndef GLCAMERA_H
#define GLCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../ICamera.h"



class GLCamera : public ICamera
{
private:

	int m_viewPort;
        int m_width;
        int m_height;

	float				m_FOVy, m_aspectRatio, m_nearZ, m_farZ;
	Vector3				m_position, m_forward, m_right, m_up;
	Float4x4			m_view;
        glm::mat4                       m_projection;
        void*                           m_vpProjection;
        void*                           m_vpView;
	void UpdateView();
	void UpdateProjection();
	void UpdateFrustum();

public:

	GLCamera(void);
	GLCamera(float fovy, int width,int height, float nearZ, float farZ);
	~GLCamera(void);



	//void Rotate(float pitch, float rotY) = 0;

	//void Pitch(float angle) = 0;
	//void RotateY(float angle) = 0;

#pragma region Get Functions
	void* GetView();
	void* GetProjection();

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

	void SetPosition(Vector3 position)			{ m_position = position; UpdateView(); }
	//void SetPosition(float x, float y, float z) = 0;
	void SetForward(Vector3 forward);
	//void SetForward(float x, float y, float z) = 0;
        void SetLookAt(Vector3 _target){}
	//void SetLookAt(Vector3 target) = 0;

	//void SetLookAt(float x, float y, float z) = 0;

	void SetViewPort(int X, int Y, int Width, int Height)
	{
		//m_ViewPort.TopLeftX = (float)X;
		//m_ViewPort.TopLeftY = (float)Y;
		//m_ViewPort.Width = (float)Width;
		//m_ViewPort.Height = (float)Height;
	}

#pragma endregion



};




#endif
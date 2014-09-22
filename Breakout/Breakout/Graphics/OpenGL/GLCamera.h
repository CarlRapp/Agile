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
	glm::vec3			m_position, m_forward, m_right, m_up;
        glm::mat4                       m_projection,m_view;
        void*                           m_vpProjection;
        void*                           m_vpView;
	void UpdateView();
	void UpdateProjection();
	void UpdateFrustum();
        glm::mat4 m_identityMatrix = glm::mat4 {1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1};

public:

	GLCamera(void);
	GLCamera(float fovy, int width,int height, float nearZ, float farZ);
	~GLCamera(void);



	//void Rotate(float pitch, float rotY) = 0;

	//void Pitch(float angle) = 0;
	//void RotateY(float angle) = 0;

#pragma region Get Functions
	MATRIX4* GetView();
	MATRIX4* GetProjection();

	glm::vec3 GetPosition()		{ return m_position; }
	glm::vec3 GetForward()		{ return m_forward; }
	glm::vec3 GetRight()		{ return m_right; }
	glm::vec3 GetUp()		{ return m_up; }

	//BoundingFrustum GetFrustum() = 0;

	void* GetViewPort() { return &m_viewPort; }
#pragma endregion

#pragma region Set Functions
	//void SetFOVY(float fovy) = 0;
	//void SetAspectRatio(float aspectRatio) = 0;
	//void SetNearZ(float nearZ) = 0;
	//void SetFarZ(float farZ) = 0;

        void Move(glm::vec3 _move);
        void Move(float _move);
	void SetPosition(glm::vec3 position) { m_position = position; UpdateView(); }
	//void SetPosition(float x, float y, float z) = 0;
	void SetForward(glm::vec3 forward);
	//void SetForward(float x, float y, float z) = 0;
        void SetLookAt(glm::vec3 _target);
	//void SetLookAt(Vector3 target) = 0;
//	void Strafe(float delta);
//	void Yaw(float angle);
//	void Pitch(float angle);
//	void Roll(float angle);
//	void RotateY(float angle);
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
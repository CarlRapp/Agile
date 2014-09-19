#pragma once
#ifndef ICAMERA_H
#define ICAMERA_H

#include "../stdafx.h"

class ICamera
{
private:


public:

	virtual ~ICamera(void) {};

	static ICamera* GetICamera(float _fov, int _width,int _height, float _nearZ, float _farZ);


	//virtual void Rotate(float pitch, float rotY) = 0;

	//virtual void Pitch(float angle) = 0;
	//virtual void RotateY(float angle) = 0;

#pragma region Get Functions
	virtual void* GetView() = 0;
	virtual void* GetProjection() = 0;

	virtual VECTOR3 GetPosition() = 0;
	//virtual Vector3 GetForward() = 0;
	//virtual Vector3 GetRight() = 0;
	//virtual Vector3 GetUp() = 0;

	//virtual BoundingFrustum GetFrustum() = 0;

	virtual void* GetViewPort() = 0;
#pragma endregion

#pragma region Set Functions
	//virtual void SetFOVY(float fovy) = 0;
	//virtual void SetAspectRatio(float aspectRatio) = 0;
	//virtual void SetNearZ(float nearZ) = 0;
	//virtual void SetFarZ(float farZ) = 0;

	virtual void SetPosition(VECTOR3 _position) = 0;
	virtual void Move(VECTOR3 _move) = 0;
        virtual void Move(float _move) = 0;
	//virtual void SetPosition(float x, float y, float z) = 0;
		virtual void SetForward(VECTOR3 _forward) = 0;
	//virtual void SetForward(float x, float y, float z) = 0;

		virtual void SetLookAt(VECTOR3 _target) = 0;

	//virtual void SetLookAt(float x, float y, float z) = 0;

	virtual void SetViewPort(int _x, int _y, int _width, int _height) = 0;

#pragma endregion



};




#endif
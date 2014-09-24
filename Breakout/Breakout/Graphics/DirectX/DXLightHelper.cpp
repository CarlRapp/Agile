#include "DXLightHelper.h"


DirectX::XMMATRIX DirectionalLight::GetViewMatrix(DirectX::BoundingFrustum& frustum, float offset)
{
	DirectX::BoundingSphere sphere;
	sphere.CreateFromFrustum(sphere, frustum);

	DirectX::XMVECTOR center = DirectX::XMLoadFloat3(&sphere.Center);
	DirectX::XMVECTOR dir = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(gpuLight->Direction.x, gpuLight->Direction.y, gpuLight->Direction.z));
	dir = DirectX::XMVector3Normalize(dir);

	offset += sphere.Radius;
	//XMVECTOR pos	= center - (offset * dir);

	float to = 2 * offset;
	
	DirectX::XMVECTOR pos = DirectX::XMVectorSubtract(center, DirectX::XMVectorMultiply(dir, DirectX::XMLoadFloat(&to)));


	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

	DirectX::XMFLOAT3 temp;

	DirectX::XMStoreFloat3(&temp, center);

	if (DirectX::XMVector3Equal(dir, DirectX::XMVectorZero())) //DETTA SKA ALDRIG SKE!
		dir = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(dir, DirectX::XMVectorSet(0, -1, 0, 0)))
		up = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(dir, DirectX::XMVectorSet(0, 1, 0, 0)))
		up = DirectX::XMVectorSet(0, 0, -1, 0);

	return DirectX::XMMatrixLookAtLH(pos, center, up);
}


DirectX::XMMATRIX DirectionalLight::GetProjectionMatrix(/*float nearZ, float farZ*/)
{

	float n = 1.0f;
	float f = DIRECTION_MAXRANGE;
	/*
	if (!(nearZ < 1.0f || farZ > DIRECTION_MAXRANGE || nearZ > farZ))
	{
		n = nearZ;
		f = farZ;
	}
	*/
	//XMMatrixOrthographicOffCenterLH(
	return DirectX::XMMatrixOrthographicLH(1000, 1000, n, f);
}

//void DirectionalLight::GetViewProjOBB(DirectX::BoundingFrustum& frustum, float offset, DirectX::XMFLOAT4X4& View, DirectX::XMFLOAT4X4& Proj, DirectX::BoundingOrientedBox& OBB)
//{
//
//	DirectX::XMFLOAT3 cornersW[frustum.CORNER_COUNT];
//	frustum.GetCorners(&cornersW[0]);
//
//	DirectX::XMVECTOR centerW = DirectX::XMLoadFloat3(&XMFLOAT3(0, 0, 0));
//	for each (DirectX::XMFLOAT3 corner in cornersW)
//	{
//		DirectX::XMVECTOR temp = XMLoadFloat3(&corner);
//		centerW += temp;
//	}
//	centerW /= 8;
//	
//	DirectX::XMVECTOR lightDirW = DirectX::XMLoadFloat3(&XMFLOAT3(gpuLight->Direction.x, gpuLight->Direction.y, gpuLight->Direction.z));
//	lightDirW = DirectX::XMVector3Normalize(lightDirW);
//
//	DirectX::XMVECTOR lightPosW = centerW - (frustum.Far * lightDirW);
//
//	DirectX::XMVECTOR upW = DirectX::XMVectorSet(0, 1, 0, 0);
//	if (DirectX::XMVector3Equal(lightDirW, DirectX::XMVectorSet(0, -1, 0, 0)))
//		upW = DirectX::XMVectorSet(0, 0, 1, 0);
//
//	else if (DirectX::XMVector3Equal(lightDirW, DirectX::XMVectorSet(0, 1, 0, 0)))
//		upW = DirectX::XMVectorSet(0, 0, -1, 0);
//
//	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(lightPosW, centerW, upW);
//
//	DirectX::XMFLOAT3 minV = XMFLOAT3(10000, 10000, 10000);
//	DirectX::XMFLOAT3 maxV = XMFLOAT3(-10000, -10000, -10000);
//
//	for each (DirectX::XMFLOAT3 corner in cornersW)
//	{
//		DirectX::XMVECTOR temp = XMLoadFloat3(&corner);
//
//		DirectX::XMFLOAT3 cornerV;
//		DirectX::XMStoreFloat3(&cornerV, DirectX::XMVector3TransformCoord(temp, V));
//
//		minV.x = min(cornerV.x, minV.x);
//		minV.y = min(cornerV.y, minV.y);
//		minV.z = min(cornerV.z - offset, minV.z);
//
//		maxV.x = max(cornerV.x, maxV.x);
//		maxV.y = max(cornerV.y, maxV.y);
//		maxV.z = max(cornerV.z, maxV.z);
//	}
//
//	// Ortho frustum in light space encloses scene.
//	float l = minV.x;
//	float b = minV.y;
//	float n = minV.z;
//
//	float r = maxV.x;
//	float t = maxV.y;
//	float f = maxV.z;
//
//	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
//	
//	DirectX::XMStoreFloat4x4(&View, V);
//	DirectX::XMStoreFloat4x4(&Proj, P);
//
//
//	std::vector<DirectX::XMFLOAT3> cornersV;
//	cornersV.push_back(minV);
//	cornersV.push_back(maxV);
//
//	DirectX::BoundingBox AABB;
//	DirectX::BoundingBox::CreateFromPoints(AABB, cornersV.size(), &cornersV[0], sizeof(DirectX::XMFLOAT3));
//
//	DirectX::BoundingOrientedBox::CreateFromBoundingBox(OBB, AABB);
//
//	DirectX::XMVECTOR det = XMMatrixDeterminant(V);
//	DirectX::XMMATRIX iV = XMMatrixInverse(&det, V);
//
//	OBB.Transform(OBB, iV);
//}

DirectX::XMMATRIX CalculateViewMatrix(DirectX::XMFLOAT3& Position, DirectX::XMFLOAT3& Direction)
{
	DirectX::XMVECTOR pos = XMLoadFloat3(&Position);
	DirectX::XMVECTOR dir = XMLoadFloat3(&Direction);
	dir = DirectX::XMVector3Normalize(dir);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

	if (DirectX::XMVector3Equal(dir, DirectX::XMVectorZero())) //DETTA SKA ALDRIG SKE!
		dir = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(dir, DirectX::XMVectorSet(0, -1, 0, 0)))
		up = DirectX::XMVectorSet(0, 0, 1, 0);

	else if (DirectX::XMVector3Equal(dir, DirectX::XMVectorSet(0, 1, 0, 0)))
		up = DirectX::XMVectorSet(0, 0, -1, 0);

	//XMFLOAT4X4 result;

	//XMStoreFloat4x4(&result, XMMatrixLookToLH(pos, dir, up));

	return DirectX::XMMatrixLookToLH(pos, dir, up);
}

std::vector<DirectX::XMFLOAT4X4> PointLight::GetViewMatrixes()
{
	std::vector<DirectX::XMFLOAT4X4> views;

	DirectX::XMFLOAT4X4 view;
	DirectX::XMMATRIX temp;

	//höger
	temp = CalculateViewMatrix(gpuLight->Position, DirectX::XMFLOAT3(1, 0, 0));
	DirectX::XMStoreFloat4x4(&view, temp);
	views.push_back(view);

	//upp
	temp = CalculateViewMatrix(gpuLight->Position, DirectX::XMFLOAT3(0, 1, 0));
	DirectX::XMStoreFloat4x4(&view, temp);
	views.push_back(view);

	//fram
	temp = CalculateViewMatrix(gpuLight->Position, DirectX::XMFLOAT3(0, 0, 1));
	DirectX::XMStoreFloat4x4(&view, temp);
	views.push_back(view);

	//vänster
	temp = CalculateViewMatrix(gpuLight->Position, DirectX::XMFLOAT3(-1, 0, 0));
	DirectX::XMStoreFloat4x4(&view, temp);
	views.push_back(view);

	//ner
	temp = CalculateViewMatrix(gpuLight->Position, DirectX::XMFLOAT3(0, -1, 0));
	DirectX::XMStoreFloat4x4(&view, temp);
	views.push_back(view);

	//bak
	temp = CalculateViewMatrix(gpuLight->Position, DirectX::XMFLOAT3(0, 0, -1));
	DirectX::XMStoreFloat4x4(&view, temp);
	views.push_back(view);

	return views;
}

DirectX::XMMATRIX PointLight::GetProjectionMatrix(/*float nearZ, float farZ*/)
{
	DirectX::XMMATRIX projcetion;
	float fovY = PI * 0.505f;
	float n = 0.05f;
	float f = gpuLight->Range;
/*
	if (!(nearZ < 1.0f || farZ > Range || nearZ > farZ))
	{
		n = nearZ;
		f = farZ;
	}
	*/
	return DirectX::XMMatrixPerspectiveFovLH(fovY, 1, n, f);
}

DirectX::XMMATRIX SpotLight::GetViewMatrix()
{
	return CalculateViewMatrix(gpuLight->Position, gpuLight->Direction);
	/*
	XMVECTOR pos = XMLoadFloat3(&Position);
	XMVECTOR dir = XMLoadFloat3(&Direction);	
	dir = XMVector3Normalize(dir);
	XMVECTOR up	= XMVectorSet(0, 1 , 0, 0);

	if (XMVector3Equal(dir, XMVectorZero())) //DETTA SKA ALDRIG SKE!
		dir = XMVectorSet(0, 0, 1, 0);

	else if (XMVector3Equal(dir, XMVectorSet(0, -1, 0, 0)))
		up = XMVectorSet(0, 0 , 1, 0);

	else if (XMVector3Equal(dir, XMVectorSet(0, 1, 0, 0)))
		up = XMVectorSet(0, 0 , -1, 0);

	return XMMatrixLookToLH(pos, dir, up);
	*/
}

DirectX::XMMATRIX SpotLight::GetProjectionMatrix(/*float nearZ, float farZ*/)
{
	DirectX::XMMATRIX projcetion;
	float fovY = 2 * gpuLight->angle;
	float n = 0.05f;
	float f = gpuLight->Range;
	/*
	if (!(nearZ < 1.0f || farZ > Range || nearZ > farZ))
	{
		n = nearZ;
		f = farZ;
	}
	*/
	return DirectX::XMMatrixPerspectiveFovLH(fovY, 1, n, f);
}
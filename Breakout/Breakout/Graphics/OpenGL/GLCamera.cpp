

#include "GLCamera.h"
#include <string>
#include <GL/glew.h>

GLCamera::GLCamera(float _fovy, int _width,int _height, float _nearZ, float _farZ)
{
    //m_ViewPort.MinDepth = 0.0f;
    //m_ViewPort.MaxDepth = 1.0f;
    
    m_width = _width;
    m_height = _height;
    
    m_aspectRatio = (float)m_width / (float)m_height;

    m_FOVy = _fovy;//2 * glm::atan(glm::tan(0.5f * _fovy) * m_aspectRatio);

    m_nearZ = _nearZ;
    m_farZ = _farZ;

    m_position  = Vector3(0, 0, 5);
    m_forward   = Vector3(0, 0, -1);
    m_right     = Vector3(1, 0, 0);
    m_up        = Vector3(0, 1, 0);

    UpdateView();
    UpdateProjection();
    SetForward(Vector3(0,0,-1));
}

GLCamera::~GLCamera(void)
{

}


void GLCamera::UpdateView()
{
	glm::vec3 up, right, forward, position;
	
//	memcpy(&up, &m_up, sizeof(glm::vec3));
//	memcpy(&right, &m_right, sizeof(glm::vec3));
//	memcpy(&forward, &m_forward, sizeof(glm::vec3));
//	memcpy(&position, &m_position, sizeof(glm::vec3));
        
//	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&right);
//	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&up);
//	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&forward);
//	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
        
        glm::vec3 R = glm::vec3(m_right.x,      m_right.y,      m_right.z);
	glm::vec3 U = glm::vec3(m_up.x,         m_up.y,         m_up.z);
	glm::vec3 L = glm::vec3(m_forward.x,    m_forward.y,    m_forward.z);
	glm::vec3 P = glm::vec3(m_position.x,   m_position.y,   m_position.z);

	// Keep camera's axes orthogonal to each other and of unit length.
//	L = DirectX::XMVector3Normalize(L);
//	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));
        L =  glm::normalize(L);
	U =  glm::normalize(glm::cross(L, R));
	// U, L already ortho-normal, so no need to normalize cross product.
	//R = DirectX::XMVector3Cross(U, L);
        R = glm::cross(U, L);

	// Fill in the view matrix entries.
//	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
//	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
//	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

        float x = -glm::vec3(glm::dot(P, R)).x;
	float y = -glm::vec3(glm::dot(P, U)).x;
	float z = -glm::vec3(glm::dot(P, L)).x;
        
	right   = R;
	up      = U;
	forward = L;

	m_view[0][0] = R.x;
	m_view[1][0] = R.y;
	m_view[2][0] = R.z;
	m_view[3][0] = x;

	m_view[0][1] = U.x;
	m_view[1][1] = U.y;
	m_view[2][1] = U.z;
	m_view[3][1] = y;

	m_view[0][2] = L.x;
	m_view[1][2] = L.y;
	m_view[2][2] = L.z;
	m_view[3][2] = z;

	m_view[0][3] = 0.0f;
	m_view[1][3] = 0.0f;
	m_view[2][3] = 0.0f;
	m_view[3][3] = 1.0f;
        
        //m_view = m_identityMatrix;
//            for(int i=0;i< 4;i++)
//    {
//        for(int j=0; j < 4;j++)
//            printf("%f ",m_view[i][j]);
//        printf("\n");
//    }
//    printf("VIEW SECOND\n");
}

void GLCamera::UpdateProjection()
{

    printf("FOV: %f | AspectRatio: %f | nearZ: %f | farZ: %f\n",m_FOVy,m_aspectRatio,m_nearZ,m_farZ);
    glm::mat4 proj;
    proj =glm::perspective(m_FOVy,m_aspectRatio,m_nearZ,m_farZ);
    //glm::mat4x4 proj4x4;
    //proj = m_identityMatrix;
    //DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(m_FOVy, m_aspectRatio, m_nearZ, m_farZ);
    //DirectX::XMFLOAT4X4 proj4x4;
    //DirectX::XMStoreFloat4x4(&proj4x4, proj);
    
    //proj = glm::mat4 {1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1};
        
    memcpy(&m_projection, &proj, sizeof(glm::mat4));
//    
//    for(int i=0;i< 4;i++)
//    {
//        for(int j=0; j < 4;j++)
//            printf("%f ",m_projection[i][j]);
//        printf("\n");
//    }
//    printf("PROJECTION END\n");
}

MATRIX4 *GLCamera::GetProjection()
{
//    for(int i=0;i< 4;i++)
//    {
//        for(int j=0; j < 4;j++)
//            printf("%f ",m_projection.m[i][j]);// .m[i][j]);
//        printf("\n");
//    }
//    printf("PROJECTION END\n");
    
    return &m_projection;
}

MATRIX4* GLCamera::GetView()
{
    return &m_view;
}

void GLCamera::SetForward(Vector3 forward)
{
	//glm::vec3 up, right, forward2;

	//memcpy(&up, &m_up, sizeof(glm::vec3));
	//memcpy(&right, &m_right, sizeof(glm::vec3));
	//memcpy(&forward2, &forward, sizeof(Vector3));
	//memcpy(&position, &m_position, sizeof(glm::vec3));

        glm::vec3 pos = glm::vec3(m_position.x,m_position.y,m_position.z);
	glm::vec3 direction = glm::vec3(forward.x,forward.y,forward.z);
	direction = glm::normalize(direction);
	glm::vec3 up2 = glm::vec3(0, 1, 0);
        
//	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
//	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3(&forward2);
//	direction = DirectX::XMVector3Normalize(direction);
//	DirectX::XMVECTOR up2 = DirectX::XMVectorSet(0, 1, 0, 0);

//	if (DirectX::XMVector3Equal(direction, DirectX::XMVectorZero()))
//		direction = DirectX::XMVectorSet(0, 0, 1, 0);
//
//	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, -1, 0, 0)))
//		up2 = DirectX::XMVectorSet(0, 0, 1, 0);
//
//	else if (DirectX::XMVector3Equal(direction, DirectX::XMVectorSet(0, 1, 0, 0)))
//		up2 = DirectX::XMVectorSet(0, 0, -1, 0);

       // glm::vec3 zero = glm::vec3(0,0,0);

        if (direction == glm::vec3(0,0,0))
		direction = glm::vec3(0, 0, 1);

	else if (direction == glm::vec3(0, -1, 0))
		up2 = glm::vec3(0, 0, 1);

	else if (direction == glm::vec3(0,1,0))
		up2 = glm::vec3(0, 0, -1);

        
	m_view = glm::lookAt(pos,direction,up2);

	//DirectX::XMStoreFloat4x4(&view4x4, view);
	//memcpy(&m_view, &view, sizeof(glm::mat4));
        
//        DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(pos, direction, up2);
//
//	DirectX::XMFLOAT4X4 view4x4;
//	DirectX::XMStoreFloat4x4(&view4x4, view);
//	memcpy(&m_view, &view4x4, sizeof(Float4x4));
        
	m_right = Vector3(m_view[0][0], m_view[1][0], m_view[2][0]);
	m_up = Vector3(m_view[0][1], m_view[1][1], m_view[2][1]);
	m_forward = Vector3(m_view[0][2], m_view[1][2], m_view[2][2]);
    

        
	UpdateView();
        
//    for(int i=0;i< 4;i++)
//    {
//        for(int j=0; j < 4;j++)
//            printf("%f ",m_view[i][j]);
//        printf("\n");
//    }
//    printf("VIEW FIRST\n");
}

void GLCamera::Move(Vector3 _move)
{
    m_position.x += _move.x;
    m_position.y += _move.y;
    m_position.z += _move.z;
    
    UpdateView();
    
}

void GLCamera::Move(float _move)
{
    m_position.x += (m_forward.x)*_move;
    m_position.y += (m_forward.y)*_move;
    m_position.z += (m_forward.z)*_move;
    
    UpdateView();
    
}
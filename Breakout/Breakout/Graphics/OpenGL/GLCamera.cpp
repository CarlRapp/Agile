

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
    
    m_pitch     = 0.0f;
    m_yaw       = 180.0f;
    m_sens      = 0.001f;

    m_position  = glm::vec3(0, 0, 5);
    m_forward   = glm::vec3(0, 0, -1);
    m_right     = glm::vec3(1, 0, 0);
    m_up        = glm::vec3(0, 1, 0);
    m_offset    = glm::vec3(0, 0, 0);

    UpdateView();
    UpdateProjection();
    SetForward(glm::vec3(0,0,-1));
    
}

GLCamera::~GLCamera(void)
{

}


void GLCamera::UpdateView()
{
	glm::vec3 up, right, forward; 
        glm::vec3 position = GetPosition();
	
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
	glm::vec3 P = glm::vec3(position.x,     position.y,     position.z);

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

void GLCamera::SetLookAt(glm::vec3 _target)
{
    glm::vec3 forward;
    forward.x = _target.x - GetPosition().x;
    forward.y = _target.y - GetPosition().y;
    forward.z = _target.z - GetPosition().z;

    SetForward(forward);
}

void GLCamera::SetForward(glm::vec3 forward)
{
	//glm::vec3 up, right, forward2;

	//memcpy(&up, &m_up, sizeof(glm::vec3));
	//memcpy(&right, &m_right, sizeof(glm::vec3));
	//memcpy(&forward2, &forward, sizeof(Vector3));
	//memcpy(&position, &m_position, sizeof(glm::vec3));

        glm::vec3 pos = GetPosition();
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

        
	m_view = glm::lookAt(pos,pos+direction,up2);

	//DirectX::XMStoreFloat4x4(&view4x4, view);
	//memcpy(&m_view, &view, sizeof(glm::mat4));
        
//        DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(pos, direction, up2);
//
//	DirectX::XMFLOAT4X4 view4x4;
//	DirectX::XMStoreFloat4x4(&view4x4, view);
//	memcpy(&m_view, &view4x4, sizeof(Float4x4));
        
	m_right = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);
	m_up = glm::vec3(m_view[0][1], m_view[1][1], m_view[2][1]);
	m_forward = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);
    

        
	UpdateView();
        
}

void GLCamera::Move(glm::vec3 _move)
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

float randf(float _min, float _max)
{
	return _min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_max - _min)));
}

void GLCamera::AddShakeEffect(int _id, float _minOffset, float _maxOffset, float _frequency, float _time)
{
	CameraShake cs;
	cs.time = 0.0f;
	cs.loop = _time < 0;
        
        if (cs.loop)
            _time = 50 * _frequency;
        
	

	ShakeKeyFrame first;
	first.offset = glm::vec3(0, 0, 0);
	first.startTime = 0;
	
	cs.shakeKeyFrames.push_back(first);

	int numPoints = _frequency * _time;
	float timePerPoint = 1.0f / _frequency;
	for (int i = 1; i < numPoints; ++i)
	{
		ShakeKeyFrame keyFrame;

		keyFrame.offset = NORMALIZE(glm::vec3(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f)))
			* randf(_minOffset, _maxOffset);
		keyFrame.startTime = i * _time / (float)numPoints;
		cs.shakeKeyFrames.push_back(keyFrame);
	}

	ShakeKeyFrame last;
	last.offset = glm::vec3(0, 0, 0);
	last.startTime = _time;


	cs.shakeKeyFrames.push_back(last);

	m_cameraShakes[_id] = cs;
}

void GLCamera::RemoveShake(int _id)
{
	m_cameraShakes.erase(_id);
}

void GLCamera::Update(float _dt)
{
	glm::vec3 newOffset(0, 0, 0);

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
				glm::vec3 offset = cs->shakeKeyFrames[j].offset * lerpFactor + cs->shakeKeyFrames[j - 1].offset * (1 - lerpFactor);

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

//void GLCamera::Strafe(float delta) 
//{
//	m_position += delta * m_right;
//}
//
//void GLCamera::Roll(float angle) 
//{
//	glm::mat4 R = glm::rotate(angle, m_forward);
//	glm::vec4 r = (R * glm::vec4(m_right, 0));
//	m_right.x = r.x;
//	m_right.y = r.y;
//	m_right.z = r.z;
//	glm::vec4 u = (R * glm::vec4(m_up, 0));
//	m_up.x = u.x;
//	m_up.y = u.y;
//	m_up.z = u.z;
//}
//
//void GLCamera::Pitch(float angle) 
//{
//	glm::mat4 R = glm::rotate(angle, m_right);
//	glm::vec4 l = (R * glm::vec4(m_forward, 0));
//	m_forward.x = l.x;
//	m_forward.y = l.y;
//	m_forward.z = l.z;
//	glm::vec4 u = (R * glm::vec4(m_up, 0));
//	m_up.x = u.x;
//	m_up.y = u.y;
//	m_up.z = u.z;
//}
//
//void GLCamera::Yaw(float angle) 
//{
//	glm::mat4 R = glm::rotate(angle, m_up);
//	glm::vec4 l = (R * glm::vec4(m_forward, 0));
//	m_forward.x = l.x;
//	m_forward.y = l.y;
//	m_forward.z = l.z;
//	glm::vec4 r = (R * glm::vec4(m_right, 0));
//	m_right.x = r.x;
//	m_right.y = r.y;
//	m_right.z = r.z;
//}
//
//void GLCamera::RotateY(float angle) 
//{   
//	glm::mat4 R = glm::rotate(angle, glm::vec3(0, 1, 0));
//	glm::vec4 l = (glm::vec4(m_forward, 0) * R);
//	m_forward.x = l.x;
//	m_forward.y = l.y;
//	m_forward.z = l.z;
//	glm::vec4 r = (glm::vec4(m_right, 0) * R);
//	m_right.x = r.x;
//	m_right.y = r.y;
//	m_right.z = r.z;
//	glm::vec4 u = (glm::vec4(m_up, 0) * R);
//	m_up.x = u.x;
//	m_up.y = u.y;
//	m_up.z = u.z;
//}
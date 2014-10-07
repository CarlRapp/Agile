#ifndef _GLPARTICLESYSTEM_H_
#define _GLPARTICLESYSTEM_H_
#include "../IGraphics.h"
#include "GLTextureManager.h"
#include "GLShaderHandler.h"
#include <time.h>

#include <GL/glew.h> 
#include <GL/gl.h> 
#include <GL/glu.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>

//#include <cmath>
//#include <math.h>

using glm::vec3;

class GLParticleSystem
{
private:
	//ShaderHParticleSystemandler* mShader;
	GLuint *m_progHandle;
	
	GLuint m_particleArray[2];
	GLuint m_feedback[2]; // Transform feedback objects
	GLuint m_posBuf[2]; // Position buffers (A and B)
	GLuint m_velBuf[2]; // Velocity buffers (A and B)
	GLuint m_startTime[2]; // Start time buffers (A and B)
	GLuint m_initVelBuf;
	GLuint m_initPosBuf;

	GLuint m_subRoutineUpdate, m_subRoutineRender;

	int m_noParticles, m_type;
	int m_drawBuf;
	vec3 m_pos, m_accel;
	float m_size, m_lifeTime;

	void CreateFire();
	void CreateSmoke();

public:
	GLParticleSystem(char* _type, const vec3 _pos, int _nParticles, float _lifeTime, float _size, GLuint *_texHandle, GLuint *_shaderProg);
	~GLParticleSystem();

	GLuint *m_textureHandle;

	//bool InitParticleSystem(const vec3 _pos, int _nParticles, char* _texPath);
	void Render(ShaderHandler *particleProg, int _deltaTimeMillis, int _elapsedTime);
	vec3 GetWorldPos(){ return m_pos; }
	vec3 GetAccel(){ return m_accel; }
	void SetAccel(vec3 acc){ m_accel = acc; }
};

#endif
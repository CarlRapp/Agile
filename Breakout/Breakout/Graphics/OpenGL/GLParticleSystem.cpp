#include "GLParticleSystem.h"


GLParticleSystem::GLParticleSystem(std::string _type, vec3 *_pos, int _nParticles, float _lifeTime, float _size, GLuint *_texHandle, GLuint *_shaderProg)
{
	m_pos = _pos;
	m_noParticles = _nParticles;
	m_size = _size;
	m_lifeTime = _lifeTime;
	m_textureHandle = _texHandle;
	m_progHandle = _shaderProg;
	m_drawBuf = 1;

	if (_type == "fire")
		CreateFire();
	else if (_type == "smoke")
		CreateSmoke();
        
        

	//set uniforms?
	m_subRoutineUpdate = glGetSubroutineIndex(*_shaderProg, GL_VERTEX_SHADER, "update");
	m_subRoutineRender = glGetSubroutineIndex(*_shaderProg, GL_VERTEX_SHADER, "render");
        
        printf("m_subRoutineUpdate: %i,     m_subRoutineRender: %i \n ", m_subRoutineUpdate, m_subRoutineRender);
}


GLParticleSystem::~GLParticleSystem()
{
    
}

void GLParticleSystem::CreateFire()
{
	// Create and allocate buffers A and B for posBuf, velBuf
	// and startTime
	//�
        elapsedTime = 0.0f;
	m_accel = vec3(0, 0, 0);
	m_type = 0;
	vec3 v(0.0f);
	float velocity, theta, phi;
	float mtime = 0.0f, rate = (m_lifeTime / (float)m_noParticles); //*6.0f;//0.00075f;

	GLfloat *posData = new GLfloat[m_noParticles * 3];
	GLfloat *velData = new GLfloat[m_noParticles * 3];
	GLfloat *timeData = new GLfloat[m_noParticles];
	GLfloat *initVelData = new GLfloat[m_noParticles * 3];
	GLfloat *initPosData = new GLfloat[m_noParticles * 3];

	srand(time(NULL));
	for (GLuint i = 0; i < m_noParticles; i++) {
		posData[3 * i] = (float)(rand() % 9) - 4;
		posData[3 * i + 1] = 0.0;
		posData[3 * i + 2] = (float)(rand() % 9 - 4);

		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)M_PI / 6.0f, (float)(rand() % 101) / 100);
		phi = glm::mix(0.0f, (float)(2 * M_PI), (float)(rand() % 101) / 100);

		v.x = glm::sin(theta) * glm::cos(phi) * 0.1; 
		if ((posData[3 * i] < 0 && v.x < 0) || (posData[3 * i] > 0 && v.x > 0))
			v.x *= -1;
		v.y = glm::cos(theta) * 0.4;
		v.z = glm::sin(theta) * glm::sin(phi) * 0.1;
		if ((posData[3 * i + 2] < 0 && v.z < 0) || (posData[3 * i + 2] > 0 && v.z > 0))
			v.z *= -1;

		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(1.25f, 1.5f, (float)(rand() % 101) / 100);
		v = v * velocity * 0.1f;
		velData[3 * i] = v.x;
		velData[3 * i + 1] = v.y;
		velData[3 * i + 2] = v.z;

		timeData[i] = mtime;
		mtime += rate;

		//printf("velData[i]: %f, %f, %f \n", velData[3 * i], velData[3 * i + 1], velData[3 * i + 2]);
		printf("timeData[i]: %f \n", timeData[i]);

	}
	initVelData = velData;
	initPosData = posData;

	GLuint vboHandles[8];
	glGenBuffers(8, vboHandles);
	m_posBuf[0] = vboHandles[0];
	m_posBuf[1] = vboHandles[1];
	m_velBuf[0] = vboHandles[2];
	m_velBuf[1] = vboHandles[3];
	m_startTime[0] = vboHandles[4];
	m_startTime[1] = vboHandles[5];
	m_initVelBuf = vboHandles[6];
	m_initPosBuf = vboHandles[7];

	//Create the two vertex arrays
	glGenVertexArrays(2, m_particleArray);
	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(m_particleArray[i]);

		// enable "vertex attribute arrays"
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	glBindVertexArray(0);

	//kanske ska vara glBufferData h�r ------   OBS!  --------
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), posData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), velData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glBufferData(GL_ARRAY_BUFFER, m_noParticles * sizeof(float), timeData, GL_STATIC_DRAW);

	}
	glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
	glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), initVelData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
	glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), initPosData, GL_STATIC_DRAW);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, m_feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[1]);
}

void GLParticleSystem::CreateSmoke()
{
	// Create and allocate buffers A and B for posBuf, velBuf
	// and startTime
	//�
	m_accel = vec3(0.0);
	m_type = 1;
	vec3 v(0.0f);
	float velocity, theta, phi;
	float mtime = 0.0f, rate = (m_lifeTime / (float)m_noParticles)*3.0f;//0.00075f;

	GLfloat *posData = new GLfloat[m_noParticles * 3];
	GLfloat *velData = new GLfloat[m_noParticles * 3];
	GLfloat *timeData = new GLfloat[m_noParticles];
	GLfloat *initVelData = new GLfloat[m_noParticles * 3];
	GLfloat *initPosData = new GLfloat[m_noParticles * 3];

	srand(time(NULL));
	for (GLuint i = 0; i < m_noParticles; i++) {
		posData[3 * i] = (float)(rand() % 21)/10 - 1.0;
		posData[3 * i + 1] = 0.0;
		posData[3 * i + 2] = (float)(rand() % 21)/10 - 1.0;

		// Pick the direction of the velocity
		theta = glm::mix(0.0f, (float)M_PI / 6.0f, (float)(rand() % 101) / 100);
		phi = glm::mix(0.0f, (float)(2 * M_PI), (float)(rand() % 101) / 100);
		v.x = glm::sin(theta) * glm::cos(phi) * 0.2;
		v.y = glm::cos(theta) * 0.6;
		v.z = glm::sin(theta) * glm::sin(phi) * 0.2;
		// Scale to set the magnitude of the velocity (speed)
		velocity = glm::mix(1.25f, 1.5f, (float)(rand() % 101) / 100);
		v = v * velocity;
		velData[3 * i] = v.x;
		velData[3 * i + 1] = v.y;
		velData[3 * i + 2] = v.z;

		timeData[i] = mtime;
		mtime += rate;
	}
	initVelData = velData;
	initPosData = posData;

	GLuint vboHandles[8];
	glGenBuffers(8, vboHandles);
	m_posBuf[0] = vboHandles[0];
	m_posBuf[1] = vboHandles[1];
	m_velBuf[0] = vboHandles[2];
	m_velBuf[1] = vboHandles[3];
	m_startTime[0] = vboHandles[4];
	m_startTime[1] = vboHandles[5];
	m_initVelBuf = vboHandles[6];
	m_initPosBuf = vboHandles[7];

	//Create the two vertex arrays
	glGenVertexArrays(2, m_particleArray);
	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(m_particleArray[i]);

		// enable "vertex attribute arrays"
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}
	glBindVertexArray(0);

	//kanske ska vara glBufferData h�r ------   OBS!  --------
	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), posData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_velBuf[i]);
		glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), velData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_startTime[i]);
		glBufferData(GL_ARRAY_BUFFER, m_noParticles * sizeof(float), timeData, GL_STATIC_DRAW);

	}
	glBindBuffer(GL_ARRAY_BUFFER, m_initVelBuf);
	glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), initVelData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_initPosBuf);
	glBufferData(GL_ARRAY_BUFFER, m_noParticles * 3 * sizeof(float), initPosData, GL_STATIC_DRAW);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, m_feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, m_startTime[1]);
}

void GLParticleSystem::Render(ShaderHandler *particleProg, float dt)
{
	/////////// Update pass ////////////////
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &m_subRoutineUpdate);
	// Set the uniforms: H and Time
	//�
        dt *= 1000.f;
        elapsedTime += dt;
        
	particleProg->SetUniformV("Time", elapsedTime);
	particleProg->SetUniformV("DeltaTime", dt);
	particleProg->SetUniformV("ParticleLifetime", m_lifeTime);
	particleProg->SetUniformV("Size", m_size);
	particleProg->SetUniformV("Type", m_type);
	particleProg->SetUniformV("Accel", m_accel);
        
//        printf("elTime: %f \n", elapsedTime);
//        printf("DeltaTime: %f \n", dt);
//        printf("ParticleLifetime: %f \n", m_lifeTime);
//        printf("Size: %f \n", m_size);
//        printf("Accel: %f, %f, %f \n\n\n", m_accel.x, m_accel.y, m_accel.z);
        
//        GLint *values;
//        glGetProgramStageiv(*m_progHandle, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINES, values);
//        printf("Active subs values: %i \n", *values);

	// Disable rendering
	glEnable(GL_RASTERIZER_DISCARD);

	// Bind the feedback object for the buffers to be drawn next
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_feedback[m_drawBuf]);

	// Draw points from input buffer with transform feedback
	glBeginTransformFeedback(GL_POINTS);
	glBindVertexArray(m_particleArray[1 - m_drawBuf]);
	glDrawArrays(GL_POINTS, 0, m_noParticles);
	glEndTransformFeedback();

	// Enable rendering
	glDisable(GL_RASTERIZER_DISCARD);

	//////////// Render pass ///////////////
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &m_subRoutineRender);
	//glClear(GL_COLOR_BUFFER_BIT);
	// Initialize uniforms for transformation matrices if needed
	//�
	// Un-bind the feedback object.
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	// Draw the sprites from the feedback buffer
	glBindVertexArray(m_particleArray[m_drawBuf]);
	glDrawArrays(GL_POINTS, 0, m_noParticles);
	// Swap buffers
	m_drawBuf = 1 - m_drawBuf;
        
}

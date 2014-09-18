#include "GLGraphics.h"
#include <iostream>
#include "../../Storage/FileManager.h"

GLGraphics::GLGraphics(void)
{

}

GLGraphics::~GLGraphics(void)
{
    Free();
    while(m_shaders.size() > 0)
        m_shaders.pop_back();
}

bool GLGraphics::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode)
{
    
    m_screenHeight = _height;
    m_screenWidth = _width;
    m_window = new GLWindow();
    return m_window->InitWindow(_width, _height);
}



bool GLGraphics::Init3D(DisplayMode _displayMode) 
{ 
    m_window->InitGL();

    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    
    m_program = glCreateProgram();
    
    Shader* a = new Shader("standard_vertex.glsl",GL_VERTEX_SHADER);
    Shader* b = new Shader("standard_fragment.glsl",GL_FRAGMENT_SHADER);
    
    //m_shaders.push_back(new Shader("standard_vertex.glsl",GL_VERTEX_SHADER));

    
    glAttachShader(m_program, a->GetShaderID());
    glAttachShader(m_program, b->GetShaderID());
    
    a=NULL;
    b=NULL;
    
    //m_shaders.push_back(new Shader("standard_fragment.glsl",GL_FRAGMENT_SHADER));
   // glAttachShader(m_program,GL_FRAGMENT_SHADER);
    
    glBindAttribLocation(m_program, 0, "m_position");
    glBindAttribLocation(m_program, 1, "m_normal");
    //const char* uniform_name;
    //uniform_name = "mvp";
    //uniform_mvp = glGetUniformLocation(program, uniform_name);

    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &link_ok);
    
    if (!link_ok) 
    {
        fprintf(stderr, "glLinkProgram:");
        //print_log(m_program);
        return 0;
    }
    
    printf("OpenGL version supported by this platform: (%s) \n", glGetString(GL_VERSION));
    std::cout << "Initialize 3D with error: " << glGetError() << "\n";
    //LoadModel("triangle");
    LoadModel("sphere");
    return true; 
} 

int kk=1;

void GLGraphics::LoadModel(std::string _path)
{
    ModelData* data = FileManager::GetInstance().LoadModel(GetFile(_path,MODEL_ROOT));

    if(!data)
    {
        printf("Loadmodel failed: %s\n",_path.c_str());
        return;
    }
    
    for (std::vector<Group*>::iterator groupIt = data->Groups.begin(); groupIt != data->Groups.end(); ++groupIt)
    {
        ModelRenderInfo* newModel = new ModelRenderInfo;
        
        m_models.push_back(newModel);
       
        int index = m_models.size()-1;
        
        m_models[index]->bufferNormalID = index;
        m_models[index]->bufferVertexID = index;
        
        int floatSize = (*groupIt)->triangles.size() * 9;
        
        float* vertexArray = new float[floatSize];
        float* normalArray = new float[floatSize];

        int i = 0;
        for (std::vector<Triangle>::iterator triangleIt = (*groupIt)->triangles.begin(); triangleIt != (*groupIt)->triangles.end(); ++triangleIt)
        {
            //Dest,Source,Size
            memcpy(&vertexArray[3*i], &(*triangleIt).Vertices[0].Position, sizeof(VECTOR3));
            memcpy(&normalArray[3*i], &(*triangleIt).Vertices[0].Normal, sizeof(VECTOR3));
            memcpy(&vertexArray[3*(i+1)], &(*triangleIt).Vertices[1].Position, sizeof(VECTOR3));
            memcpy(&normalArray[3*(i+1)], &(*triangleIt).Vertices[1].Normal, sizeof(VECTOR3));
            memcpy(&vertexArray[3*(i+2)], &(*triangleIt).Vertices[2].Position, sizeof(VECTOR3));
            memcpy(&normalArray[3*(i+2)], &(*triangleIt).Vertices[2].Normal, sizeof(VECTOR3));

            i += 3;
        }
        printf("Vertices: %d\n", i);
        glGenBuffers(kk, &m_models[index]->bufferVertexID);
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->bufferVertexID);
        glBufferData(GL_ARRAY_BUFFER, floatSize * sizeof(float), vertexArray, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glGenBuffers(kk, &m_models[index]->bufferNormalID);
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->bufferNormalID);
        glBufferData(GL_ARRAY_BUFFER, floatSize * sizeof(float), normalArray, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    kk++;
    std::cout << "Loadmodel finish: " << _path << " with error: " << glGetError() << "\n";
}

void GLGraphics::Update() 
{

}

void GLGraphics::Resize(int _width, int _height) 
{
    m_screenWidth = _width;
    m_screenHeight = _height;
    glViewport(0, 0, m_screenWidth, m_screenHeight);
}

void GLGraphics::Free()
{
    for(int i = m_models.size()-1; i > -1;i--)
    {
        glDeleteBuffers(1, &m_models[i]->bufferNormalID);
        glDeleteBuffers(1, &m_models[i]->bufferVertexID);
        
        m_models.pop_back();
    }
    
    glDeleteProgram(m_program);
}

float t;

void GLGraphics::Render(ICamera* _camera) 
{ 

    glClearColor(0.4, 0.4, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(m_program);

    glBindVertexArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 1);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    t+=0.001f;
    //TODO:: GET ENTITY
    m_testLightPos.z = glm::sin(t)*5+10;
    m_testLightPos.x = glm::cos(t)*5+10;
    m_testMatrix = ROTATE(m_testMatrix,glm::sin(t)*2,glm::vec3(1.0f,0.f,0.f));
    //glm::rotate(m_testMatrix,glm::sin(t)*2,glm::vec3(1.0f,0.f,0.f));
    glm::vec3 gg = glm::vec3(1.0f,0.f,0.f);
    //TRANSLATE(&m_testMatrix,&gg,0,0);
    //m_testMatrix = TRANSLATE(m_testMatrix,gg,0,0);
    //m_testMatrix = MATRIX4.TRANSLATE();
    
    GLint testLight = glGetUniformLocation(m_program, "m_testLight" );
    glUniform3f(testLight, m_testLightPos.x,m_testLightPos.y,m_testLightPos.z);
    
    GLint model = glGetUniformLocation(m_program, "m_matModel" );
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m_testMatrix));
    
    MATRIX4* temp1 = (MATRIX4*)_camera->GetProjection();
    
    GLint projection = glGetUniformLocation(m_program, "m_matProj" );
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(*temp1));
    
    temp1 = (MATRIX4*)_camera->GetView();

    GLint view = glGetUniformLocation(m_program, "m_matView" );
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(*temp1));

    glDrawArrays(GL_TRIANGLES, 0, 2160);

    
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glUseProgram(0);
    
    SDL_GL_SwapBuffers( );
    
}

int GLGraphics::SetUniformV(const char* _variable, float _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform1fv(location, 1, &_value);
	else
		return 1;

	return 0;
}

int GLGraphics::SetUniformV(const char* _variable, glm::vec3 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform3fv(location, 1, &_value[0]);
	else
		return 1;

	return 0;
}

int GLGraphics::SetUniformV(const char* _variable, glm::vec4 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform4fv(location, 1, &_value[0]);
	else
		return 1;

	return 0;
}

int GLGraphics::SetUniformV(const char* _variable, glm::mat3 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniformMatrix3fv(location, 1, GL_FALSE, &_value[0][0]);
	else 
		return 1;

	return 0;
}

int GLGraphics::SetUniformV(const char* _variable, glm::mat4 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniformMatrix4fv(location, 1, GL_FALSE, &_value[0][0]);
	else
		return 1;

	return 0;
}

int GLGraphics::SetUniformV(const char* _variable, int _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform1i(location, _value);
	else return 1;

	return 0;
}
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
    
    glBindAttribLocation(m_program, 0, "coord3d");
    glBindAttribLocation(m_program, 1, "v_color");

    
    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &link_ok);
    
    if (!link_ok) 
    {
        fprintf(stderr, "glLinkProgram:");
        //print_log(m_program);
        return 0;
    }
  
    std::cout << "Initialize 3D with error: " << glGetError() << "\n";
    LoadModel("triangle.obj");
    return true; 
} 

void GLGraphics::LoadModel(std::string _path)
{
    ModelData* data = FileManager::GetInstance().LoadModel(_path);
    
    printf("NVertx: %d\n", data->GetVertices().size());
    float* vertexArray = new float[data->GetVertices().size()*3];
    float* colorArray = new float[data->GetVertices().size()*3];
    
    for(int i = 0; i < data->GetVertices().size(); ++i)
    {
        //Dest,Source,Size
        memcpy(&vertexArray[3*i], &data->GetVertices()[i].Position, sizeof(Vector3));
        memcpy(&colorArray[3*i], &data->GetVertices()[i].Normal, sizeof(Vector3));
//        colorArray[3*i] = data->GetVertices()[i].Normal.X;
//        colorArray[3*i+1] = data->GetVertices()[i].Normal.Y;
//        colorArray[3*i+2] = data->GetVertices()[i].Normal.Z;
        
        //printf("i=%d (%f, %f, %f)\n", i, vertexArray[3*i],vertexArray[3*i+1],vertexArray[3*i+2]);
    }
    

    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ARRAY_BUFFER, 3 * data->GetVertices().size() * sizeof(float), vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glGenBuffers(1, &vbo_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
   glBufferData(GL_ARRAY_BUFFER, 3 * data->GetVertices().size() * sizeof(float), colorArray, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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
    glDeleteProgram(m_program);
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_colors);
//    glDeleteBuffers(1, &ibo_cube_elements);
}

void GLGraphics::Render(ICamera* _camera) 
{ 
    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(m_program);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(ibo_cube_elements);
    
    glDrawArrays(GL_TRIANGLES, 0, 9);

    glBindVertexArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
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
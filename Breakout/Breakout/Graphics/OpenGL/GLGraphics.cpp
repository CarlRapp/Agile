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
    
    LoadModel("triangle");
    //LoadModel("sphere");
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
        m_models.push_back(new ModelRenderInfo());
       
        int index = m_models.size()-1;

        int floatCount = (*groupIt)->triangles.size() * 3 * 3;
        
        float* vertexArray = new float[floatCount];
        float* normalArray = new float[floatCount];

        int i = 0;
        for (std::vector<Triangle>::iterator triangleIt = (*groupIt)->triangles.begin(); triangleIt != (*groupIt)->triangles.end(); ++triangleIt)
        {
            //Dest,Source,Size
            memcpy(&vertexArray[3*i], &(*triangleIt).Vertices[0].Position, sizeof(glm::vec3));
            memcpy(&normalArray[3*i], &(*triangleIt).Vertices[0].Normal, sizeof(glm::vec3));
            memcpy(&vertexArray[3*(i+1)], &(*triangleIt).Vertices[1].Position, sizeof(glm::vec3));
            memcpy(&normalArray[3*(i+1)], &(*triangleIt).Vertices[1].Normal, sizeof(glm::vec3));
            memcpy(&vertexArray[3*(i+2)], &(*triangleIt).Vertices[2].Position, sizeof(glm::vec3));
            memcpy(&normalArray[3*(i+2)], &(*triangleIt).Vertices[2].Normal, sizeof(glm::vec3));

            i += 3;
        }
        printf("Vertices: %d [Index: %d]\n", i, index);
        m_models[index]->vertices = i;
        m_models[index]->name = _path;
        
        m_testMatrices.push_back(glm::mat4(1.0f));
        m_testMatrices.push_back(glm::mat4(1.0f));
        
        m_testMatrices[0] = glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,1.0f,1.0f));
        m_testMatrices[1] = glm::translate(glm::mat4(1.0f),glm::vec3(-1.0f,-1.0f,-1.0f));
        
        int pos             = glGetAttribLocation(m_program, "m_position");
        int pad1            = glGetAttribLocation(m_program, "pad1");
        int normal          = glGetAttribLocation(m_program, "m_normal");
        int pad2            = glGetAttribLocation(m_program, "pad2");
        int color           = glGetAttribLocation(m_program, "m_color");
        int matrix          = glGetAttribLocation(m_program, "m_matModel");
        
        GLuint VBOHandles[4];
	glGenBuffers(4, VBOHandles);
        float padData = 0;
	// "Bind" (switch focus to) first buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[0]); 
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), vertexArray, GL_DYNAMIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]); 
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float), &padData, GL_DYNAMIC_DRAW);
        
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), normalArray, GL_DYNAMIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[3]); 
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float), &padData, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[2]);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), &m_testColor, GL_DYNAMIC_DRAW);
//        
        glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[3]);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &m_testMatrices[index], GL_DYNAMIC_DRAW);
	// create 1 VAO
	glGenVertexArrays(1, &m_models[index]->bufferVAOID);
	glBindVertexArray(m_models[index]->bufferVAOID);


        
	// enable "vertex attribute arrays"
	glEnableVertexAttribArray(pos);         // position     1
        glEnableVertexAttribArray(pos+1);       // pad          2
	glEnableVertexAttribArray(normal);      // normal       3
        glEnableVertexAttribArray(normal+1);    // pad          4
        glEnableVertexAttribArray(color);       // color        5

        glEnableVertexAttribArray(matrix);      //matrix        6
        glEnableVertexAttribArray(matrix+1);    //matrix        7
        glEnableVertexAttribArray(matrix+2);    //matrix        8
        glEnableVertexAttribArray(matrix+3);    //matrix        9
        

	// vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[0]);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

        //pad
        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
        glVertexAttribPointer(pos+1, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(sizeof(glm::vec3)));

        //normal
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
        //pad
        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[3]);
        glVertexAttribPointer(normal+1, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(sizeof(glm::vec3)));

        //color
        glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[2]);
        glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glVertexAttribDivisor(color, 1);

        //model matrix
        glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[3]);
        glVertexAttribPointer(matrix, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 0));
        glVertexAttribDivisor(matrix, 1);
        glVertexAttribPointer(matrix+1, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 1));
        glVertexAttribDivisor(matrix+1, 1);
        glVertexAttribPointer(matrix+2, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 2));
        glVertexAttribDivisor(matrix+2, 1);
        glVertexAttribPointer(matrix+3, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 3));
        glVertexAttribDivisor(matrix+3, 1);


        glBindVertexArray(0); // disable VAO
        glUseProgram(0); // disable shader programme
    }
    
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
       // glDeleteBuffers(1, &m_models[i]->bufferNormalID);
       // glDeleteBuffers(1, &m_models[i]->bufferVertexID);
        
        m_models.pop_back();
    }
    
    for(int i = m_testMatrices.size()-1;i >-1;i--)
    {
        m_testMatrices.pop_back();
    }
    
    glDeleteProgram(m_program);
}

float t;

void GLGraphics::Render(ICamera* _camera) 
{ 
    int instances = 2;
    
    glClearColor(0.4, 0.4, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    
    
    int vertices = ModelToRender();
    glUseProgram(m_program);
    // Activate instancing program
    

    //CameraToRedner();
    LightsToRender();
    CameraToRender(_camera);
    // Render INSTANCE_COUNT objects
    
    glDrawArraysInstanced(GL_TRIANGLES,0,vertices,instances);
    glBindVertexArray(0);

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
    
    SDL_GL_SwapBuffers( );
    
}

int GLGraphics::ModelToRender()
{
    t+=0.001f;
    int instances = 2;
    m_testMatrices[0] *= glm::rotate(glm::mat4(1.0f),t,glm::vec3(0.0f,0.0f,1.0f));
    m_testMatrices[1] *= glm::rotate(glm::mat4(1.0f),t,glm::vec3(0.0f,1.0f,0.0f));

    ModelRenderInfo* MRI;

    MRI = m_models[0];
    
    glBindBuffer(GL_ARRAY_BUFFER,4);

    glm::mat4* matrices = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    //std::cout << "Mapping error: " << glGetError() << "\n";

    for(int i=0; i< instances; i++)
    {
        matrices[i] = m_testMatrices[i];

//        for(int k=0;k< 4;k++)
//        {
//            for(int j=0; j < 4;j++)
//                printf("    %f",matrices[i][k][j]););
//            printf("\n");
//        }
//        printf("END\n");
    }
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    glBindVertexArray(MRI->bufferVAOID);

    return MRI->vertices;
}

void GLGraphics::LightsToRender()
{
    m_testLightPos.z = glm::sin(t)*5+10;
    m_testLightPos.x = glm::cos(t)*5+10;
    GLint testLight = glGetUniformLocation(m_program, "m_testLight" );
    glUniform3f(testLight, m_testLightPos.x,m_testLightPos.y,m_testLightPos.z);
}

void GLGraphics::CameraToRender(ICamera* _camera)
{
    glm::mat4* temp1 = _camera->GetProjection();
    
    GLint projection = glGetUniformLocation(m_program, "m_matProj" );
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(*temp1));
    
    temp1 = _camera->GetView();

    GLint view = glGetUniformLocation(m_program, "m_matView" );
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(*temp1));
}

void GLGraphics::AddRenderObject(std::string _path, MATRIX4 _world)
{
    
}

void GLGraphics::RemoveObject(int _id)
{
    
}

void GLGraphics::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose)
{
    
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
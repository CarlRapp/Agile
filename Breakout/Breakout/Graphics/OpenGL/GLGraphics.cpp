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

    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    GLint major, minor;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("\033[35mGL Vendor : %s\n", vendor);
    printf("\033[35mGL Renderer : %s\n", renderer);
    printf("\033[35mGL Version (string) : %s\n", version);
    printf("\033[35mGL Version (integer) : %d.%d\n", major, minor);
    printf("\033[35mGLSL Version : %s\n\033[30m", glslVersion);
    
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
        printf("\033[31mUnable to link shader\n\033[30m");
        return 0;
    }
    
    std::cout << "Initialize 3D Finish";
        if(glGetError())
    std::cout << "\033[31m with error: " << glGetError();
    std::cout << "\n\033[30m";
    
    //LoadModel("triangle");
    //LoadModel("sphere");
    return true; 
} 

int kk=1;

void GLGraphics::LoadModel(std::string _path)
{
    ModelData* data = FileManager::GetInstance().LoadModel(GetFile(_path,MODEL_ROOT));

    if(!data)
    {
        printf("\033[31mLoadmodel failed: %s\n\033[30m",_path.c_str());
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
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[3]);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        
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
    
    std::cout << "Loadmodel finish: " << _path;
    if(glGetError())
    std::cout << "\033[31m with error: " << glGetError();
    std::cout << "\n\033[30m";
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
        m_models[i]->instances.clear();
        glDeleteBuffers(5,&m_models[i]->bufferVAOID);
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
    
    
    
    glUseProgram(m_program);
    
    LightsToRender();
    
    CameraToRender(_camera);
    
    RenderInstanced();
    
    //RenderStandard();
    
    glUseProgram(0);
    
    SDL_GL_SwapBuffers( );
    
}

int GLGraphics::RenderStandard()
{
    for(int i=0;i<m_models.size();i++)
    {
        glBindVertexArray(m_models[i]->bufferVAOID);

        glDrawArrays(GL_TRIANGLES,0,m_models[i]->vertices);
    
        glBindVertexArray(0);
    }
}

int GLGraphics::RenderInstanced()
{
    t+=0.001f;

    ModelRenderInfo* MRI;
    
    for(int i=0; i< m_models.size();i++)
    {
        MRI = m_models[i];
        int instances = m_models[i]->instances.size();
        //printf("%s\n",m_models[i]->name.c_str());
        glBindBuffer(GL_ARRAY_BUFFER,(i+1)*4);

        glm::mat4* matrices = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        //std::cout << "Mapping error: " << glGetError() << "\n";

        int j = 0;

        for (std::map < int, ModelInstance*>::const_iterator insIt = m_models[i]->instances.begin(); insIt != m_models[i]->instances.end(); ++insIt)
        {
            matrices[j] = *insIt->second->world;
            j++;
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glBindVertexArray(MRI->bufferVAOID);

        glDrawArraysInstanced(GL_TRIANGLES,0,MRI->vertices,instances);

        glBindVertexArray(0);
    }
    
    return 1;
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

void GLGraphics::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose)
{

    for(int i=0; i < m_models.size();i++)
    {
        if (m_models[0]->instances.count(_id) !=0)
                return;
    }

    int newModelID = -1;
    
    for(int i=0; i < m_models.size();i++)
    {
//        if(!(i == m_models.size()-1))
//        if(m_models[i]->name != _model)
//        {
//            continue;
//        }
//        
        if(m_models[i]->name == _model)
        {
            newModelID = i;
            printf("Model exists %s , skipping LoadModel\n",_model.c_str());
            break;
        }
    }
    
    if(newModelID == -1)
    {
        newModelID = m_models.size();
        printf("Model not found, using LoadModel\n");
        LoadModel(_model);
    }
    
    ModelInstance *mi = new ModelInstance();

    mi->world = _world;
    
    mi->worldInverseTranspose = _worldInverseTranspose;

    m_models[newModelID]->instances.insert(pair<int, ModelInstance*>(_id, mi));
    

}

void GLGraphics::RemoveObject(int _id)
{
    for(int i=0; i< m_models.size();i++)
    {
        m_models[i]->instances.erase(_id);
    }
}

//int GLGraphics::SetUniformV(const char* _variable, float _value)
//{
//	//	Set as current program
//	glUseProgram(m_program);
//
//	//	Get pointer for variable
//	int location = glGetUniformLocation(m_program, _variable);
//	if (location >= 0)
//		glUniform1fv(location, 1, &_value);
//	else
//		return 1;
//
//	return 0;
//}
//
//int GLGraphics::SetUniformV(const char* _variable, glm::vec3 _value)
//{
//	//	Set as current program
//	glUseProgram(m_program);
//
//	//	Get pointer for variable
//	int location = glGetUniformLocation(m_program, _variable);
//	if (location >= 0)
//		glUniform3fv(location, 1, &_value[0]);
//	else
//		return 1;
//
//	return 0;
//}
//
//int GLGraphics::SetUniformV(const char* _variable, glm::vec4 _value)
//{
//	//	Set as current program
//	glUseProgram(m_program);
//
//	//	Get pointer for variable
//	int location = glGetUniformLocation(m_program, _variable);
//	if (location >= 0)
//		glUniform4fv(location, 1, &_value[0]);
//	else
//		return 1;
//
//	return 0;
//}
//
//int GLGraphics::SetUniformV(const char* _variable, glm::mat3 _value)
//{
//	//	Set as current program
//	glUseProgram(m_program);
//
//	//	Get pointer for variable
//	int location = glGetUniformLocation(m_program, _variable);
//	if (location >= 0)
//		glUniformMatrix3fv(location, 1, GL_FALSE, &_value[0][0]);
//	else 
//		return 1;
//
//	return 0;
//}
//
//int GLGraphics::SetUniformV(const char* _variable, glm::mat4 _value)
//{
//	//	Set as current program
//	glUseProgram(m_program);
//
//	//	Get pointer for variable
//	int location = glGetUniformLocation(m_program, _variable);
//	if (location >= 0)
//		glUniformMatrix4fv(location, 1, GL_FALSE, &_value[0][0]);
//	else
//		return 1;
//
//	return 0;
//}
//
//int GLGraphics::SetUniformV(const char* _variable, int _value)
//{
//	//	Set as current program
//	glUseProgram(m_program);
//
//	//	Get pointer for variable
//	int location = glGetUniformLocation(m_program, _variable);
//	if (location >= 0)
//		glUniform1i(location, _value);
//	else return 1;
//
//	return 0;
//}
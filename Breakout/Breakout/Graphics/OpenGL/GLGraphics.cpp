#include "GLGraphics.h"
#include <iostream>
#include "../../Storage/FileManager.h"

#define SHADE_TEXT 1

GLGraphics::GLGraphics(void)
{

}

GLGraphics::~GLGraphics(void)
{
    Free();
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
    
    
//-----------Creating shader programs----------------------------------------------
    m_standardShaderProgram.CreateShaderProgram();
    
    m_standardShaderProgram.AddShader("standard_vertex.glsl",GL_VERTEX_SHADER);
    m_standardShaderProgram.AddShader("standard_fragment.glsl",GL_FRAGMENT_SHADER);
    m_standardShaderProgram.AddShader("explode_geometry.glsl",GL_GEOMETRY_SHADER);

    m_standardShaderProgram.LinkShaderProgram();

//------------------------------------------------------------------------------------
    m_shader2Dprogram.CreateShaderProgram();
    
    m_shader2Dprogram.AddShader("2Dshader_vertex.glsl",GL_VERTEX_SHADER);
    m_shader2Dprogram.AddShader("2Dshader_fragment.glsl",GL_FRAGMENT_SHADER);
    
    m_shader2Dprogram.LinkShaderProgram();
    
//-----------------------------------------------------------------------------------------
    
    m_textProgram.CreateShaderProgram();
    
    m_textProgram.AddShader("text_vertex.glsl",GL_VERTEX_SHADER);
    m_textProgram.AddShader("text_fragment.glsl",GL_FRAGMENT_SHADER);
    
    m_textProgram.LinkShaderProgram();
    
//-----------------------------------------------------------------------------------------
    
    glEnable(GL_BLEND);
    
    float aspect = m_screenHeight/m_screenWidth;
    
    m_tX = 0.0;
    m_tY = 0.0;
    m_tW = (8.f*58)/(m_screenWidth);
    m_tH = 8.0f/(m_screenHeight);
    
    m_texManager.LoadLetters();
    //LoadTexture("TEXT");
    Add2DTexture(999, "TEXT", &m_tX, &m_tY, &m_tW, &m_tH); 
    
//------------------------------------------------------------------------------------
    m_fireParticlesProgram.CreateShaderProgram();
    
    m_fireParticlesProgram.AddShader("explosionParticles_vertex.glsl",GL_VERTEX_SHADER);
    m_fireParticlesProgram.AddShader("explosionParticles_fragment.glsl",GL_FRAGMENT_SHADER);
    
    const char * outputNames[] = { "Position", "Velocity", "StartTime" };
    glTransformFeedbackVaryings(m_fireParticlesProgram.GetProgramHandle(), 3, outputNames, GL_SEPARATE_ATTRIBS);

    m_fireParticlesProgram.LinkShaderProgram();
    m_texManager.Load2DTexture("fire3.png", GL_TEXTURE0);
//------------------------------------------------------------------------------------
    m_trailParticlesProgram.CreateShaderProgram();
    
    m_trailParticlesProgram.AddShader("trailParticles_vertex.glsl",GL_VERTEX_SHADER);
    m_trailParticlesProgram.AddShader("trailParticles_fragment.glsl",GL_FRAGMENT_SHADER);
    
    //const char * outputNames[] = { "Position", "Velocity", "StartTime" };
    glTransformFeedbackVaryings(m_trailParticlesProgram.GetProgramHandle(), 3, outputNames, GL_SEPARATE_ATTRIBS);

    m_trailParticlesProgram.LinkShaderProgram();
    m_texManager.Load2DTexture("circle_blue.png", GL_TEXTURE0);
//------------------------------------------------------------------------------------
    
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SPRITE);
   
    int err = glGetError();
    
    std::cout << "Initialize 3D Finish";
        if(err)
    std::cout << "\033[31m with error: " << err;
    std::cout << "\n\033[30m";

    glEnable(GL_DEPTH_TEST);
    return true; 
} 

void GLGraphics::LoadModel(std::string _path)
{
    m_standardShaderProgram.UseProgram();
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

        int floatCount = (*groupIt)->triangles->size() * 3 * 3;
        
        float* vertexArray = new float[floatCount];
        float* normalArray = new float[floatCount];
        float* texCoordArray = new float[(int)(*groupIt)->triangles->size() * 3 * 2];
        
        
        int i = 0;
        for (std::vector<Triangle>::iterator triangleIt = (*groupIt)->triangles->begin(); triangleIt != (*groupIt)->triangles->end(); ++triangleIt)
        {
            //Dest,Source,Size
            memcpy(&vertexArray[3*i], &(*triangleIt).Vertices[0].Position, sizeof(glm::vec3));
            memcpy(&normalArray[3*i], &(*triangleIt).Vertices[0].Normal, sizeof(glm::vec3));
            memcpy(&texCoordArray[2*i], &(*triangleIt).Vertices[0].Texture, sizeof(glm::vec2));
            
            texCoordArray[2*i +1] = -texCoordArray[2*i +1];
            
            memcpy(&vertexArray[3*(i+1)], &(*triangleIt).Vertices[1].Position, sizeof(glm::vec3));
            memcpy(&normalArray[3*(i+1)], &(*triangleIt).Vertices[1].Normal, sizeof(glm::vec3));
            memcpy(&texCoordArray[2*(i+1)], &(*triangleIt).Vertices[1].Texture, sizeof(glm::vec2));
            
            texCoordArray[2*(i+1) + 1] = -texCoordArray[2*(i+1) + 1];
            
            memcpy(&vertexArray[3*(i+2)], &(*triangleIt).Vertices[2].Position, sizeof(glm::vec3));
            memcpy(&normalArray[3*(i+2)], &(*triangleIt).Vertices[2].Normal, sizeof(glm::vec3));
            memcpy(&texCoordArray[2*(i+2)], &(*triangleIt).Vertices[2].Texture, sizeof(glm::vec2));
            
            texCoordArray[2*(i+2) + 1] = -texCoordArray[2*(i+2) + 1];
            
            //memcpy(&normalArray[3*(i+2)], &(*triangleIt).Vertices[2].Texture, sizeof(glm::vec3));

            i += 3;
        }
        printf("Vertices: %d [Index: %d]\n", i, index);
        m_models[index]->vertices = i;
        m_models[index]->name = _path;
        
        if((*groupIt)->material)
        {
            m_texManager.Load2DTexture((*groupIt)->material->Map_Kd, GL_TEXTURE0);
            m_models[index]->texHandle = m_texManager.GetTexture((*groupIt)->material->Map_Kd);
          //  printf("MATERIAL exists, texture: %s \n", (*groupIt)->material->Map_Kd.c_str());
            m_models[index]->MaterialKs = (*groupIt)->material->Ks[0];
            m_models[index]->MaterialNs = (*groupIt)->material->Ns;
        }
        else
        {
            m_texManager.Load2DTexture("whitePixel.png", GL_TEXTURE0);
            m_models[index]->texHandle = m_texManager.GetTexture("whitePixel.png");
           // printf("NO MATERIAL FILE: using %s \n", "whitePixel.png");
            m_models[index]->MaterialKs = 0.0f;
            m_models[index]->MaterialNs = 1.0f;
        }
        
        m_testMatrices.push_back(glm::mat4(1.0f));
        
        int pos             = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "m_position");
        int pad1            = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "pad1");
        int normal          = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "m_normal");
        int explosion       = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "m_explosion");
        int color           = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "m_color");
        int matrix          = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "m_matModel");
        int texCoord        = glGetAttribLocation(m_standardShaderProgram.GetProgramHandle(), "m_texCoord");
        
	glGenBuffers(6, m_models[index]->buffers);
 
	// "Bind" (switch focus to) first buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[0]); 
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), vertexArray, GL_STATIC_READ);
        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]); 
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float), &padData, GL_DYNAMIC_DRAW);
        
	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), normalArray, GL_STATIC_READ);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[2]); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[3]);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), NULL, GL_STATIC_READ);

        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[4]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[5]); 
        glBufferData(GL_ARRAY_BUFFER, (*groupIt)->triangles->size() * 3 * 2 * sizeof(float), texCoordArray, GL_STATIC_DRAW);
        
	// create 1 VAO
	glGenVertexArrays(1, &m_models[index]->bufferVAOID);
	glBindVertexArray(m_models[index]->bufferVAOID);
        
	// enable "vertex attribute arrays"     
	glEnableVertexAttribArray(pos);         // position     0
        glEnableVertexAttribArray(pos+1);       // pad          1
	glEnableVertexAttribArray(normal);      // normal       2
        glEnableVertexAttribArray(explosion);    // explosion   3
        glEnableVertexAttribArray(color);       // color        4

        glEnableVertexAttribArray(matrix);      //matrix        5
        glEnableVertexAttribArray(matrix+1);    //matrix        6
        glEnableVertexAttribArray(matrix+2);    //matrix        7
        glEnableVertexAttribArray(matrix+3);    //matrix        8
        
        glEnableVertexAttribArray(texCoord);    //matrix        9

	// vertex
	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[0]);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        //pad
        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
        glVertexAttribPointer(pos+1, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(sizeof(glm::vec3)));

        //normal
	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[1]);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
        //explosion
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[2]);
        glVertexAttribPointer(explosion, 1, GL_FLOAT, GL_FALSE, 0, NULL);
        glVertexAttribDivisor(explosion, 1);

        //color
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[3]);
        glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glVertexAttribDivisor(color, 1);
        

        //model matrix
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[4]);
        glVertexAttribPointer(matrix, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 0));
        glVertexAttribDivisor(matrix, 1);
        glVertexAttribPointer(matrix+1, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 1));
        glVertexAttribDivisor(matrix+1, 1);
        glVertexAttribPointer(matrix+2, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 2));
        glVertexAttribDivisor(matrix+2, 1);
        glVertexAttribPointer(matrix+3, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 3));
        glVertexAttribDivisor(matrix+3, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[5]);
	glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);


        glBindVertexArray(0); // disable VAO
        glUseProgram(0); // disable shader programme
    }
    
    int err = glGetError();
    
    std::cout << "Loadmodel finish: " << _path;
    if(err)
    std::cout << "\033[31m with error: " << err;
    std::cout << "\n\033[30m";
}

void GLGraphics::LoadTexture(std::string _path)
{
    m_texManager.Load2DTexture(_path, GL_TEXTURE0);
}

void GLGraphics::Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height)
{
    if(m_TextureInstances.find(_id) != m_TextureInstances.end())
    {
        return;
    }
    m_shader2Dprogram.UseProgram();
    m_texManager.Load2DTexture(_path, GL_TEXTURE0);
    m_TextureInstances.insert(pair<int, TextureInfo*>(_id, new TextureInfo(m_texManager.GetTexture(_path), _x, _y, _width, _height)));
    
    if(m_TextureInstances.size() <= 1)
    {
        //buffra datan till 2D shader
        float positionData[] = {
            -1.0, -1.0,
            -1.0, 1.0,
            1.0, -1.0,
            -1.0, 1.0,
            1.0, 1.0,
            1.0, -1.0 };

        float texCoordData[] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f };

        int nrOfPoints = 12;

        GLuint m_2DVBOs[2];
        glGenBuffers(2, m_2DVBOs);

        // "Bind" (switch focus to) first buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[0]); 
        glBufferData(GL_ARRAY_BUFFER, nrOfPoints * sizeof(float), positionData, GL_STATIC_DRAW);


        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, nrOfPoints * sizeof(float), texCoordData, GL_STATIC_DRAW);

        // create 1 VAO
        glGenVertexArrays(1, &m_2DVAO);
        glBindVertexArray(m_2DVAO);

        // enable "vertex attribute arrays"
        glEnableVertexAttribArray(0); // position
        glEnableVertexAttribArray(1); // texCoord

        // map index 0 to position buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[0]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[1]);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);


        glBindVertexArray(0); // disable VAO
        glUseProgram(0); // disable shader programme
    //glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(2, m_2DVBOs);
    
    int err = glGetError();
    
    std::cout << "Add2DTexture ";
    if(err)
    std::cout << "\033[31m with error: " << err;
    std::cout << "\n\033[30m";
    }
}

void GLGraphics::Remove2DTexture(int _id)
{
    delete(m_TextureInstances[_id]);
    m_TextureInstances.erase(_id);
}

void GLGraphics::AddParticleEffect(int _id, std::string _effect, VECTOR3 *_pos, VECTOR3 *_vel)
{
    if(_effect == "fire")
    {
        m_particleEffects.insert(pair<int, GLParticleSystem*>(_id, new GLParticleSystem("fire", _pos, 40, 500, 85.f, 
                                                                                    m_texManager.GetTexturePointer("fire3.png"), m_fireParticlesProgram.GetProgramHandlePointer())));
    }
    else if(_effect == "trail")
    {
        m_particleEffects.insert(pair<int, GLParticleSystem*>(_id, new GLParticleSystem("trail", _pos, 300, 1000, 35.f, 
                                                                                    m_texManager.GetTexturePointer("circle_blue.png"), m_trailParticlesProgram.GetProgramHandlePointer())));
    }
}
        
void GLGraphics::RemoveParticleEffect(int _id)
{
    delete(m_particleEffects[_id]);
    m_particleEffects.erase(_id);
}

void GLGraphics::Update(float _dt) 
{
    for(int i = m_textObjects.size()-1; i >= 0; --i)
    {
        if(m_textObjects[i].text == NULL)
        {
            //9999 = no effect
            if(m_textObjects[i].effectCopy != 9999.0f)
                m_textObjects[i].effectCopy -= _dt*10;
            else
            {
                if(m_textObjects[i].kill)
                    m_textObjects.erase(m_textObjects.begin() +(i));
            }

            if(m_textObjects[i].effectCopy < -10)
            {  
                m_textObjects.erase(m_textObjects.begin() +(i));
            }
        }
    }
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
        m_models[i]->instances.clear();
        glDeleteBuffers(6,&m_models[i]->bufferVAOID);
        m_models.pop_back();
    }
    
    for(int i = m_testMatrices.size()-1;i >-1;i--)
    {
        m_testMatrices.pop_back();
    }
    
    for(std::map<int,LightInfo*>::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
    {
        delete(it->second);
    }
    m_lights.clear();
    
    for(std::map<int,TextureInfo*>::iterator it = m_TextureInstances.begin(); it != m_TextureInstances.end(); ++it)
    {
        delete(it->second);
    }
    m_TextureInstances.clear();
    m_texManager.Free();
    glDeleteVertexArrays(1, &m_2DVAO);
    
    m_standardShaderProgram.~ShaderHandler();
    m_shader2Dprogram.~ShaderHandler();
 
//    for(int i=0;i < m_letters64.size(); i++)
//    {
//        m_letters64.pop_back();
//    }
    
//    for(int i=m_letters256.size() - 1;i >= 0; i--)
//    {
//        free(m_letters256[i]);
//        m_letters256[i] = nullptr;
//        m_letters256.pop_back();
//    }

    for(int i=0; i < m_textObjects.size();i++)
    {
        m_textObjects.pop_back();
    }
    
    for(std::map<int,GLParticleSystem*>::iterator it = m_particleEffects.begin(); it != m_particleEffects.end(); ++it)
    {
        delete(it->second);
    }
    m_particleEffects.clear();
    
    printf("Graphics memory cleared\n");
}

    float t;

void GLGraphics::Render(float _dt, ICamera* _camera) 
{ 
    glClearColor(0.05, 0.05, 0.05, 1.0);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    m_standardShaderProgram.UseProgram();
    
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    
    UpdateLights();
    
    CameraToRender(_camera);
    
    RenderInstanced(_camera);
    
    Render2D();
    
    RenderParticles(_dt, _camera);
    
    glUseProgram(0);

    for(int i=0; i < m_textObjects.size();i++)
    {
        if(!m_textObjects[i].kill)
            RenderText(m_textObjects[i].text,m_textObjects[i].scale,m_textObjects[i].color,m_textObjects[i].x,m_textObjects[i].y,m_textObjects[i].effect,m_textObjects[i].kill);
        else
            RenderText(&m_textObjects[i].textCopy,&m_textObjects[i].scaleCopy,&m_textObjects[i].colorCopy,&m_textObjects[i].xCopy,&m_textObjects[i].yCopy,&m_textObjects[i].effectCopy,&m_textObjects[i].kill);
    }
    
    SDL_GL_SwapBuffers( );
}

void GLGraphics::RenderParticles(float dt, ICamera* _camera)
{
	
	glEnable(GL_BLEND);
        glEnable(GL_POINT_SPRITE);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        
	for(std::map<int,GLParticleSystem*>::iterator it = m_particleEffects.begin(); it != m_particleEffects.end(); ++it)
	{
            ShaderHandler *tmp;
            if(it->second->GetName() == "fire")
                tmp = &m_fireParticlesProgram;
            else if(it->second->GetName() == "trail")
                tmp = &m_trailParticlesProgram;
            
            tmp->UseProgram();
            glActiveTexture(GL_TEXTURE0);

            glUniformMatrix4fv(glGetUniformLocation(tmp->GetProgramHandle(), "ProjectionMatrix"), 1, GL_FALSE, &(*_camera->GetProjection())[0][0]);
            
            glBindTexture(GL_TEXTURE_2D, *it->second->m_textureHandle);

            //printf("worldPos particles: %f, %f, %f \n\n", m_particlesFire->GetWorldPos().x, m_particlesFire->GetWorldPos().y, m_particlesFire->GetWorldPos().z);
            glm::mat4 Model = glm::translate(*(it->second->GetWorldPos()));
            glm::mat4 viewMatrix = *_camera->GetView();

            GLuint location = glGetUniformLocation(tmp->GetProgramHandle(), "Model");	//gets the UniformLocation
            if (location >= 0){ glUniformMatrix4fv(location, 1, GL_FALSE, &Model[0][0]); }
            
            location = glGetUniformLocation(tmp->GetProgramHandle(), "View");	//gets the UniformLocation
            if (location >= 0){ glUniformMatrix4fv(location, 1, GL_FALSE, &viewMatrix[0][0]); }

            it->second->Render(tmp, dt);
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glUseProgram(0);
}

void GLGraphics::RenderText(std::string* _text,float* _scale, glm::vec3* _color,float* _x, float* _y,float* _effect,bool kill)
{
    if(_text == NULL)
        return;
    
    GLvoid* v;
    
    std::string text = (*_text);
    float scale = (*_scale);
    float x = (*_x);
    float y = (*_y);
    
    float width;
    float height;
    
    float effect = *_effect;
    
    if(effect == 9999.0f)
        effect = 1.0f;
    
    width = 0.00625*scale;
    height = 0.01111*scale;
    
    int sign = (kill) ? -1 : 1;

    float r_ = _color->x;
    float g_ = _color->y;
    float b_ = _color->z;
    
    m_textProgram.UseProgram();

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(m_2DVAO);
    glBindTexture(GL_TEXTURE_2D, m_TextureInstances.find(999)->second->TexHandle);

    GLint colorLocation = glGetUniformLocation(m_textProgram.GetProgramHandle(), "m_color" );
    glUniform3f(colorLocation, r_,g_,b_);

    for(int i= 0; i < text.size();i++)
    {
        x = ((*_x) + width*i) * (effect);
        glViewport((GLint)(x * m_screenWidth), (GLint)(y * m_screenHeight), (GLsizei)(m_screenWidth * width), (GLsizei)(m_screenHeight * height));
        GLint letterLocation = glGetUniformLocation(m_textProgram.GetProgramHandle(), "m_letter" );
        glUniform1i(letterLocation, text.at(i)-32);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //x += width;
    }
    glBindVertexArray(0);
    glActiveTexture(0);
}

void GLGraphics::AddTextObject(int _id, std::string *_text, float *_x, float *_y, float *_scale, VECTOR3 *_color, float *_effect)
{
    for(int i = 0; i < m_textObjects.size();i++)
    {
        if(m_textObjects[i].id == _id)
        {
            m_textObjects.erase(m_textObjects.begin() + i);
            printf("\033[31mWARNING, ADDED TEXTOBJECT WITH EXISTING ID: %d \033[30m\n",_id);

        }
    }
    
    TextObject textObject;
    textObject.text = _text;
    textObject.scale = _scale;
    textObject.color = _color;
    textObject.x = _x;
    textObject.y = _y;
    textObject.id = _id;
    textObject.effect = _effect;
    textObject.kill = false;
    m_textObjects.push_back(textObject);
    
}

void GLGraphics::RemoveTextObject(int _id)
{
    for(int i = 0; i < m_textObjects.size();i++)
    {
        if(m_textObjects[i].id == _id)
        {
            //When textobject is removed from world, copy data to graphics to play out-effect
            m_textObjects[i].textCopy   = *m_textObjects[i].text;
            m_textObjects[i].scaleCopy  = *m_textObjects[i].scale;
            m_textObjects[i].colorCopy  = *m_textObjects[i].color;
            m_textObjects[i].xCopy      = *m_textObjects[i].x;
            m_textObjects[i].yCopy      = *m_textObjects[i].y;
            m_textObjects[i].effectCopy = *m_textObjects[i].effect;
            
            m_textObjects[i].text   =NULL;
            m_textObjects[i].scale  =NULL;
            m_textObjects[i].color  =NULL;
            m_textObjects[i].x      =NULL;
            m_textObjects[i].y      =NULL;
            m_textObjects[i].effect =NULL;
            
            m_textObjects[i].kill = true;
            break;
        }
    }
}


void GLGraphics::Render2D()
{
    //printf("Entering Render2D");
    m_shader2Dprogram.UseProgram();
    
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    for(std::map<int,TextureInfo*>::iterator it = m_TextureInstances.begin(); it != m_TextureInstances.end(); ++it)
    {
        //fulhax blockerar text-texturen
        if(it->first == 999)
            continue;
        
        glBindVertexArray(m_2DVAO);
        glBindTexture(GL_TEXTURE_2D, it->second->TexHandle);
        //set viewPort (resterande TextureInfo-variabler)
        glViewport((GLint)(*it->second->X * m_screenWidth), (GLint)(*it->second->Y * m_screenHeight), (GLsizei)(*it->second->Width * m_screenWidth), (GLsizei)(*it->second->Height * m_screenHeight));
        //printf("X: %d  Y: %d    Width: %d    Height: %d \n\n", (GLint)(*it->second->X * m_screenWidth), (GLint)(*it->second->Y * m_screenHeight), (GLsizei)(*it->second->Width * m_screenWidth), (GLsizei)(*it->second->Height * m_screenHeight));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glActiveTexture(0);
    glEnable(GL_DEPTH_TEST);
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

int GLGraphics::RenderInstanced(ICamera* _camera)
{
    t+=0.001f;

    ModelRenderInfo* MRI;
    
    m_standardShaderProgram.UseProgram();
    glEnable(GL_DEPTH_TEST);
    
    m_standardShaderProgram.SetUniformV("EyePosition", vec4(_camera->GetPosition(), 1.0f));
    
    for(int i=0; i< m_models.size();i++)
    {
        m_standardShaderProgram.SetUniformV("Material.Ks", m_models[i]->MaterialKs);
        m_standardShaderProgram.SetUniformV("Material.Ns", m_models[i]->MaterialNs);
        MRI = m_models[i];
        int instances = m_models[i]->instances.size();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_models[i]->texHandle);
        
        //Update matrix buffer//
        glBindBuffer(GL_ARRAY_BUFFER,m_models[i]->buffers[4]);
        glm::mat4* matrices = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        int j = 0;

        for (std::map < int, ModelInstance*>::const_iterator insIt = m_models[i]->instances.begin(); insIt != m_models[i]->instances.end(); ++insIt)
        {
            matrices[j] = *insIt->second->world;
            //PRINTMATRIX(&matrices[j]);
            j++;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        //Update matrix buffer//
        
        //Update explosion buffer//

        glBindBuffer(GL_ARRAY_BUFFER,m_models[i]->buffers[2]);
        float* explosion = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        j = 0;

        for (std::map < int, ModelInstance*>::const_iterator insIt = m_models[i]->instances.begin(); insIt != m_models[i]->instances.end(); ++insIt)
        {
            if(insIt->second->explosion)
                explosion[j] = *insIt->second->explosion;
            else
                explosion[j] = 0.0f;

            j++;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        //Update explosion buffer<//

        glBindVertexArray(MRI->bufferVAOID);

        glDrawArraysInstanced(GL_TRIANGLES,0,MRI->vertices,instances);

        glBindVertexArray(0);
        
    }
    
    return 1;
}

void GLGraphics::AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range)
{   
    m_lights.insert(pair<int, LightInfo*>(_id, new LightInfo(_worldPos, _intensity, _color, _range)));
}

void GLGraphics::RemovePointLight(int _id)
{
    delete(m_lights[_id]);
    m_lights.erase(_id);
}

void GLGraphics::UpdateLights()
{
    int i = 0;
    for(std::map<int,LightInfo*>::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
	{
            //Light properties
            vec4 LightPosition = vec4(*it->second->Position, 1.0f);	// Light position
            
            //-----Send all the lights values------
            const char* indexStr = std::to_string(i).c_str();   //itoa(i, indexStr, 10);
            char positionStr[25], intensityStr[25], colorStr[25], rangeStr[25];
            strcpy(positionStr, "Lights[");	strcat(positionStr, indexStr);		strcat(positionStr, "].Position");
            strcpy(intensityStr, "Lights[");	strcat(intensityStr, indexStr);		strcat(intensityStr, "].Intensity");
            strcpy(colorStr, "Lights[");	strcat(colorStr, indexStr);		strcat(colorStr, "].Color");
            strcpy(rangeStr, "Lights[");	strcat(rangeStr, indexStr);		strcat(rangeStr, "].Range");

            m_standardShaderProgram.SetUniformV(positionStr, LightPosition);
            m_standardShaderProgram.SetUniformV(intensityStr, *it->second->Intensity);
            m_standardShaderProgram.SetUniformV(colorStr, *it->second->Color);
            m_standardShaderProgram.SetUniformV(rangeStr, *it->second->Range);
            
            i++;
	}
}

void GLGraphics::CameraToRender(ICamera* _camera)
{
    glm::mat4* temp1 = _camera->GetProjection();
    
    GLint projection = glGetUniformLocation(m_standardShaderProgram.GetProgramHandle(), "m_matProj" );
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(*temp1));
    
    temp1 = _camera->GetView();

    GLint view = glGetUniformLocation(m_standardShaderProgram.GetProgramHandle(), "m_matView" );
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(*temp1));
}

void GLGraphics::AddRenderObject(std::string _path, MATRIX4 _world)
{
    
}

void GLGraphics::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose,float* _explosion)
{
    for(int i=0; i < m_models.size();i++)
    {
        if (m_models[0]->instances.count(_id) !=0)
                return;
    }

    int newModelID = -1;
    
    for(int i=0; i < m_models.size();i++)
    {
        if(m_models[i]->name == _model)
        {
            newModelID = i;
            int size = 0;
            
            //Resize instance buffers only
            glBindBuffer(GL_ARRAY_BUFFER, m_models[i]->buffers[2]); 
            glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
            glBufferData(GL_ARRAY_BUFFER, size + sizeof(float), NULL, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, m_models[i]->buffers[3]);
            glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
            glBufferData(GL_ARRAY_BUFFER, size + 4 * sizeof(float), NULL, GL_STATIC_READ);

            glBindBuffer(GL_ARRAY_BUFFER, m_models[i]->buffers[4]);
            glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
            glBufferData(GL_ARRAY_BUFFER, size + 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER,0);
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
    mi->explosion = _explosion;
    mi->worldInverseTranspose = _worldInverseTranspose;

    m_models[newModelID]->instances.insert(pair<int, ModelInstance*>(_id, mi));
}

void GLGraphics::RemoveObject(int _id)
{
    for(int i = m_models.size() - 1; i>= 0; --i)
    {
//        for(int j = m_models[i]->instances.size() -1 ; j >=0 ; j --)
//        {
            if(m_models[i]->instances.find(_id) != m_models[i]->instances.end())
            {
                m_models[i]->instances.erase(m_models[i]->instances.find(_id));

                 break;
            }
       // }
    }
   // m_models[] m_models[i]->instances.find(_id)
}
/*
void LoadLetters()
{
    m_letters64.push_back(&_space);
    m_letters64.push_back(&_exclamation);
    m_letters64.push_back(&_quote);
    m_letters64.push_back(&_number);
    m_letters64.push_back(&_dollar);
    m_letters64.push_back(&_percent);
    m_letters64.push_back(&_ampersand);
    m_letters64.push_back(&_apostrophe);
    m_letters64.push_back(&_leftbrace);
    m_letters64.push_back(&_rightbrace);
    m_letters64.push_back(&_asterisk);
    m_letters64.push_back(&_plus);
    m_letters64.push_back(&_comma);
    m_letters64.push_back(&_minus);
    m_letters64.push_back(&_dot);
    m_letters64.push_back(&_slash);

    m_letters64.push_back(&_0);
    m_letters64.push_back(&_1);
    m_letters64.push_back(&_2);
    m_letters64.push_back(&_3);
    m_letters64.push_back(&_4);
    m_letters64.push_back(&_5);
    m_letters64.push_back(&_6);
    m_letters64.push_back(&_7);
    m_letters64.push_back(&_8);
    m_letters64.push_back(&_9);

    m_letters64.push_back(&_colon);
    m_letters64.push_back(&_semicolon);
    m_letters64.push_back(&_lessthan);
    m_letters64.push_back(&_equal);
    m_letters64.push_back(&_morethan);
    m_letters64.push_back(&_question);
    m_letters64.push_back(&_at);

    m_letters64.push_back(&A);
    m_letters64.push_back(&B);
    m_letters64.push_back(&C);
    m_letters64.push_back(&D);
    m_letters64.push_back(&E);
    m_letters64.push_back(&F);
    m_letters64.push_back(&G);
    m_letters64.push_back(&H);
    m_letters64.push_back(&I);
    m_letters64.push_back(&J);
    m_letters64.push_back(&K);
    m_letters64.push_back(&L);
    m_letters64.push_back(&M);
    m_letters64.push_back(&N);
    m_letters64.push_back(&O);
    m_letters64.push_back(&P);
    m_letters64.push_back(&Q);
    m_letters64.push_back(&R);
    m_letters64.push_back(&S);
    m_letters64.push_back(&T);
    m_letters64.push_back(&U);
    m_letters64.push_back(&V);
    m_letters64.push_back(&W);
    m_letters64.push_back(&X);
    m_letters64.push_back(&Y);
    m_letters64.push_back(&Z);
    
//    m_letters256.resize(m_letters64.size());
//    
//    for(int i=0; i < m_letters64.size();i++)
//    {
////        GLbyte* f;
////        f = (GLbyte*)malloc(sizeof(GLbyte[256]));
//
//        m_letters256[i] = (GLbyte(*)[256])malloc(256);
//    }
   
#ifdef SHADE_TEXT
    GLbyte temp;
    
    GLbyte shade = 'm';
    
    for(int i = 0; i < m_letters64.size();i++)
    {
        for(int j = 0; j < 64; j++)
        {
            temp = (*m_letters64[i])[j];
            if(temp == '~')
            {
//                if(j-8 > 0)
//                {
//                    if((*m_letters[i])[j-8] != '~')
//                    (*m_letters[i])[j-8] += 40;  
//                }
                
                if(j+1 < 64)
                {
                    if((*m_letters64[i])[j+1] == '~')
                    {
                        if(j+8 < 64)
                            if((*m_letters64[i])[j-8] == '~' && !((*m_letters64[i])[j-1] == '~'))
                                (*m_letters64[i])[j] -= 40;
                    }
                }
            }
            
            if(temp == 0)
            {
                int a =0;
                
                if(j-8 > 0)
                {
                    if((*m_letters64[i])[j-8] == '~')
                        a++;
                }
                
                if(j+8 < 64)
                {
                    if((*m_letters64[i])[j+8] == '~')
                        a++;
                }
                
                if(j-1 > 0)
                {
                    if((*m_letters64[i])[j-1] == '~')
                        a++;
                }
                
                if(j+1 < 64)
                {
                    if((*m_letters64[i])[j+1] == '~')
                        a++;
                }
                
                if(a>0)
                    (*m_letters64[i])[j] = ' '*a;
            }
    
            (*m_letters256[i])[j*4]     = (*m_letters64[i])[j];
            (*m_letters256[i])[j*4+1]   = (*m_letters64[i])[j];
            (*m_letters256[i])[j*4+2]   = 0;
            (*m_letters256[i])[j*4+3]   = (*m_letters64[i])[j];
        }
    }
#endif
}
*/

#include "GLGraphics.h"
#include <iostream>
#include "../../Storage/FileManager.h"
//#include <GL/glcorearb.h>
//#include <GL/glut.h>
//#include <GL/glext.h>
#include <random>

void _check_gl_error(const char *file, int line);

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)


#define COMPUTE_WORKGROUPS 16

#define CONST_CAST(x) (*(GLboolean*)&x)

#define GLEW_CONTEXT_ARG_DEF_INIT void
#define GLEW_CONTEXT_ARG_VAR_INIT
#define GLEW_CONTEXT_ARG_DEF_LIST void
#define WGLEW_CONTEXT_ARG_DEF_INIT void
#define WGLEW_CONTEXT_ARG_DEF_LIST void
#define GLXEW_CONTEXT_ARG_DEF_INIT void
#define GLXEW_CONTEXT_ARG_DEF_LIST void
GLboolean __GLEW_ARB_compute_shader = GL_TRUE;

#define glewGetProcAddress(name) (*glXGetProcAddressARB)(name)

PFNGLDISPATCHCOMPUTEPROC __glewDispatchCompute = NULL;
PFNGLDISPATCHCOMPUTEINDIRECTPROC __glewDispatchComputeIndirect = NULL;

PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC __glewDispatchComputeGroupSizeARB = NULL;

static GLboolean _glewInit_GL_ARB_compute_shader(GLEW_CONTEXT_ARG_DEF_INIT) {
    GLboolean r = GL_FALSE;

    r = ((glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC) glewGetProcAddress((const GLubyte*) "glDispatchCompute")) == NULL) || r;
    r = ((glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC) glewGetProcAddress((const GLubyte*) "glDispatchComputeIndirect")) == NULL) || r;

    return r;
}

GLGraphics::GLGraphics(void) {

}

GLGraphics::~GLGraphics(void) {
    Free();
}

bool GLGraphics::InitWindow(int _x, int _y, int _width, int _height, DisplayMode _displayMode) {
    m_screenHeight = _height;
    m_screenWidth = _width;
    m_window = new GLWindow();
    return m_window->InitWindow(_width, _height);
}

static GLuint _glewStrLen(const GLubyte* s) {
    GLuint i = 0;
    if (s == NULL) return 0;
    while (s[i] != '\0') i++;
    return i;
}

static GLuint _glewStrCLen(const GLubyte* s, GLubyte c) {
    GLuint i = 0;
    if (s == NULL) return 0;
    while (s[i] != '\0' && s[i] != c) i++;
    return (s[i] == '\0' || s[i] == c) ? i : 0;
}

static GLboolean _glewStrSame(const GLubyte* a, const GLubyte* b, GLuint n) {
    GLuint i = 0;
    if (a == NULL || b == NULL)
        return (a == NULL && b == NULL && n == 0) ? GL_TRUE : GL_FALSE;
    while (i < n && a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) i++;
    return i == n ? GL_TRUE : GL_FALSE;
}

static GLboolean _glewSearchExtension(const char* name, const GLubyte *start, const GLubyte *end) {
    const GLubyte* p;
    GLuint len = _glewStrLen((const GLubyte*) name);
    p = start;
    while (p < end) {
        GLuint n = _glewStrCLen(p, ' ');
        if (len == n && _glewStrSame((const GLubyte*) name, p, n)) return GL_TRUE;
        p += n + 1;
    }
    return GL_FALSE;
}

bool GLGraphics::Init3D(DisplayMode _displayMode) {
    m_window->InitGL();

    const GLubyte* extStart;
    const GLubyte* extEnd;

    extStart = glGetString(GL_EXTENSIONS);
    if (extStart == 0)
        extStart = (const GLubyte*) "";
    extEnd = extStart + _glewStrLen(extStart);

    CONST_CAST(GLEW_ARB_compute_shader) = _glewSearchExtension("GL_ARB_compute_shader", extStart, extEnd);
    CONST_CAST(GLEW_ARB_compute_shader) = !_glewInit_GL_ARB_compute_shader(GLEW_CONTEXT_ARG_VAR_INIT);


    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint major, minor;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("\033[35mGL Vendor : %s\n", vendor);
    printf("\033[35mGL Renderer : %s\n", renderer);
    printf("\033[35mGL Version (string) : %s\n", version);
    printf("\033[35mGL Version (integer) : %d.%d\n", major, minor);
    printf("\033[35mGLSL Version : %s\n\033[30m", glslVersion);

    //    int extCount;
    //    glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);
    //    bool found = false;
    //    for (int i = 0; i < extCount; ++i)
    //        printf("%s\n",(const char*)glGetStringi(GL_EXTENSIONS, i));

    GLint link_ok = GL_FALSE;
    GLuint vs, fs;

    //-----------Creating shader programs----------------------------------------------
    m_program = glCreateProgram();

    GenTexture();

    Shader* a = new Shader("postCompute_vs.glsl", GL_VERTEX_SHADER);
    Shader* b = new Shader("postCompute_fs.glsl", GL_FRAGMENT_SHADER);

    //Shader* a = new Shader("standard_vertex.glsl",GL_VERTEX_SHADER);
    //Shader* b = new Shader("standard_fragment.glsl",GL_FRAGMENT_SHADER);
    //Shader* c = new Shader("explode_geometry.glsl",GL_GEOMETRY_SHADER);
    Shader* c = new Shader("Compute.glsl", GL_COMPUTE_SHADER);

    glAttachShader(m_program, a->GetShaderID());
    glAttachShader(m_program, b->GetShaderID());

    a = NULL;
    b = NULL;

    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &link_ok);

    if (!link_ok) {
        printf("\033[31mUnable to link shader\n\033[30m");
        exit(0);
        return 0;
    }

    //--------------------------------QUAD--------------------------------//
    glUseProgram(m_program);
    glUniform1i(glGetUniformLocation(m_program, "srcTex"), 0);

    float vertexArray[8] = {1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f, 1.0f};

    GLuint VBOHandle[1];
    glGenBuffers(1, VBOHandle);

    glBindBuffer(GL_ARRAY_BUFFER, *VBOHandle);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof (float), vertexArray, GL_STREAM_DRAW);

    int pos = glGetAttribLocation(m_program, "pos");
    glGenVertexArrays(1, &m_computeQuad);
    glBindVertexArray(m_computeQuad);

    glEnableVertexAttribArray(pos);

    glBindBuffer(GL_ARRAY_BUFFER, *VBOHandle);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    //--------------------------------QUAD--------------------------------//
    //------------------------------------------------------------------------------------

    m_computeProgram = glCreateProgram();

    glAttachShader(m_computeProgram, c->GetShaderID());
    c = NULL;
    glLinkProgram(m_computeProgram);

    glGetProgramiv(m_computeProgram, GL_LINK_STATUS, &link_ok);

    if (!link_ok) {
        GLint maxLength = 0;
        glGetProgramiv(m_computeProgram, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_computeProgram, maxLength, &maxLength, &infoLog[0]);
        for (int i = 0; i < maxLength; i++)
            cout << infoLog[i];

        infoLog.clear();

        glDeleteProgram(m_computeProgram);

        printf("\033[31mUnable to link shader %d\n\033[30m", glGetError());
        exit(32);
        return 0;
    }
    //printf("%d\n",glGetError());
    //glUniform1i(m_computeTexture, 0);
    glUseProgram(m_computeProgram);

    int loc = glGetUniformLocation(m_computeProgram, "destTex[0]");

    glUniform1i(loc, 0);
    glUseProgram(0);

    //-----------------------------------------------------------------------------------------
    glEnable(GL_BLEND);

    m_workSizeX = (m_screenWidth / COMPUTE_WORKGROUPS);
    m_workSizeY = (m_screenHeight / COMPUTE_WORKGROUPS);

    LoadLetters();
    
    glm::mat4 matrix = glm::mat4(1.0f);
    
    m_testMatrices.push_back(matrix);

    int err = glGetError();

    std::cout << "Initialize 3D Finish";
    if (err)
        std::cout << "\033[31m with error: " << err;
    std::cout << "\n\033[30m";

    glEnable(GL_DEPTH_TEST);
    return true;
}

void GLGraphics::GenTexture() {
    m_computeTexture = 0;

    glGenTextures(1, &m_computeTexture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_computeTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_screenWidth, m_screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);

    check_gl_error();
}

void GLGraphics::LoadModel(std::string _path) {
    glUseProgram(m_computeProgram);
    ModelData* data = FileManager::GetInstance().LoadModel(GetFile(_path, MODEL_ROOT));

    if (!data) {
        printf("\033[31mLoadmodel failed: %s\n\033[30m", _path.c_str());
        return;
    }

    for (std::vector<Group*>::iterator groupIt = data->Groups.begin(); groupIt != data->Groups.end(); ++groupIt) 
    {
        m_models.push_back(new ModelRenderInfo());

        int index = m_models.size() - 1;

        int floatCount = (*groupIt)->triangles->size() * 3 * 3;

        float* vertexArray = new float[floatCount];
        float* normalArray = new float[floatCount];
        float* texCoordArray = new float[(int) (*groupIt)->triangles->size() * 3 * 2];

check_gl_error();
        int i = 0;
        for (std::vector<Triangle>::iterator triangleIt = (*groupIt)->triangles->begin(); triangleIt != (*groupIt)->triangles->end(); ++triangleIt) {
            //Dest,Source,Size
            memcpy(&vertexArray[3 * i], &(*triangleIt).Vertices[0].Position, sizeof (glm::vec3));
            memcpy(&normalArray[3 * i], &(*triangleIt).Vertices[0].Normal, sizeof (glm::vec3));
            memcpy(&texCoordArray[2 * i], &(*triangleIt).Vertices[0].Texture, sizeof (glm::vec2));

            texCoordArray[2 * i + 1] = -texCoordArray[2 * i + 1];

            memcpy(&vertexArray[3 * (i + 1)], &(*triangleIt).Vertices[1].Position, sizeof (glm::vec3));
            memcpy(&normalArray[3 * (i + 1)], &(*triangleIt).Vertices[1].Normal, sizeof (glm::vec3));
            memcpy(&texCoordArray[2 * (i + 1)], &(*triangleIt).Vertices[1].Texture, sizeof (glm::vec2));

            texCoordArray[2 * (i + 1) + 1] = -texCoordArray[2 * (i + 1) + 1];

            memcpy(&vertexArray[3 * (i + 2)], &(*triangleIt).Vertices[2].Position, sizeof (glm::vec3));
            memcpy(&normalArray[3 * (i + 2)], &(*triangleIt).Vertices[2].Normal, sizeof (glm::vec3));
            memcpy(&texCoordArray[2 * (i + 2)], &(*triangleIt).Vertices[2].Texture, sizeof (glm::vec2));

            texCoordArray[2 * (i + 2) + 1] = -texCoordArray[2 * (i + 2) + 1];

            //memcpy(&normalArray[3*(i+2)], &(*triangleIt).Vertices[2].Texture, sizeof(glm::vec3));

            i += 3;
        }
        printf("Vertices: %d [Index: %d]\n", i, index);
        m_models[index]->vertices = i;
        m_models[index]->name = _path;
check_gl_error();
        if ((*groupIt)->material) {
            m_texManager.Load2DTexture((*groupIt)->material->Map_Kd, GL_TEXTURE2);
            m_models[index]->texHandle = m_texManager.GetTexture((*groupIt)->material->Map_Kd);
            //  printf("MATERIAL exists, texture: %s \n", (*groupIt)->material->Map_Kd.c_str());
        } else {
            m_texManager.Load2DTexture("whitePixel.png", GL_TEXTURE2);
            m_models[index]->texHandle = m_texManager.GetTexture("whitePixel.png");
            // printf("NO MATERIAL FILE: using %s \n", "whitePixel.png");
        }

        m_models[index]->vertexArray = vertexArray;
        m_models[index]->normalArray = normalArray;
        m_models[index]->texCoordArray = texCoordArray;

        glUseProgram(m_computeProgram);
        check_gl_error();

        int location = glGetUniformLocation(m_computeProgram, "Models[0].pos");
        if (location >= 0)
            glUniform3fv(location, 132, m_models[index]->vertexArray);
        check_gl_error();
        
        location = glGetUniformLocation(m_computeProgram, "Models[0].normal");
        if (location >= 0)
            glUniform3fv(location, 132, m_models[index]->normalArray);
        check_gl_error();
        
        location = glGetUniformLocation(m_computeProgram, "Models[0].texCoord");
        if (location >= 0)
            glUniform2fv(location, 132, m_models[index]->texCoordArray);
        check_gl_error();
        //SetUniformV(m_computeProgram, positionStr, m_models[index]->vertexArray);

        //        int i = 0;

        //        m_computeModels.insert(pair<int, LightInfo*>(_id, new LightInfo(_worldPos, _intensity, _color, _range)));
        //        
        //        for(std::map<int,ComputeModelInfo*>::iterator it = m_lights.begin(); it != m_lights.end(); ++it)
        //	{
        //            //Light properties
        //            vec4 LightPosition = vec4(*it->second->Position, 1.0f);	// Light position
        //            
        //            //-----Send all the lights values------
        //            const char* indexStr = std::to_string(i).c_str();   //itoa(i, indexStr, 10);
        //            char positionStr[25], intensityStr[25], colorStr[25], rangeStr[25];
        //            strcpy(positionStr, "Lights[");	strcat(positionStr, indexStr);		strcat(positionStr, "].Position");
        //            strcpy(intensityStr, "Lights[");	strcat(intensityStr, indexStr);		strcat(intensityStr, "].Intensity");
        //            strcpy(colorStr, "Lights[");	strcat(colorStr, indexStr);		strcat(colorStr, "].Color");
        //            strcpy(rangeStr, "Lights[");	strcat(rangeStr, indexStr);		strcat(rangeStr, "].Range");
        //
        //            SetUniformV(m_program, positionStr, LightPosition);
        //            SetUniformV(m_program, intensityStr, *it->second->Intensity);
        //            SetUniformV(m_program, colorStr, *it->second->Color);
        //            SetUniformV(m_program, rangeStr, *it->second->Range);
        //            
        //            i++;
        //	}
        //        m_testMatrices.push_back(glm::mat4(1.0f));
        //        
        //        int pos             = glGetAttribLocation(m_computeProgram, "m_position");
        //        int pad1            = glGetAttribLocation(m_computeProgram, "pad1");
        //        int normal          = glGetAttribLocation(m_computeProgram, "m_normal");
        //        int explosion       = glGetAttribLocation(m_computeProgram, "m_explosion");
        //        int color           = glGetAttribLocation(m_computeProgram, "m_color");
        //        int matrix          = glGetAttribLocation(m_computeProgram, "m_matModel");
        //        int texCoord        = glGetAttribLocation(m_computeProgram, "m_texCoord");
        //        
        //	glGenBuffers(6, m_models[index]->buffers);
        // 
        //	// "Bind" (switch focus to) first buffer
        //	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[0]); 
        //	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), vertexArray, GL_STATIC_READ);
        //        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]); 
        //        //glBufferData(GL_ARRAY_BUFFER, sizeof(float), &padData, GL_DYNAMIC_DRAW);
        //        
        //	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[1]);
        //	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), normalArray, GL_STATIC_READ);
        //        
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[2]); 
        //        glBufferData(GL_ARRAY_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);
        //
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[3]);
        //        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), NULL, GL_STATIC_READ);
        //
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[4]);
        //        glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        //        
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[5]); 
        //        glBufferData(GL_ARRAY_BUFFER, (*groupIt)->triangles->size() * 3 * 2 * sizeof(float), texCoordArray, GL_STATIC_DRAW);
        //        
        //	// create 1 VAO
        //	glGenVertexArrays(1, &m_models[index]->bufferVAOID);
        //	glBindVertexArray(m_models[index]->bufferVAOID);
        //        
        //	// enable "vertex attribute arrays"     
        //	glEnableVertexAttribArray(pos);         // position     0
        //        glEnableVertexAttribArray(pos+1);       // pad          1
        //	glEnableVertexAttribArray(normal);      // normal       2
        //        glEnableVertexAttribArray(explosion);    // explosion   3
        //        glEnableVertexAttribArray(color);       // color        4
        //
        //        glEnableVertexAttribArray(matrix);      //matrix        5
        //        glEnableVertexAttribArray(matrix+1);    //matrix        6
        //        glEnableVertexAttribArray(matrix+2);    //matrix        7
        //        glEnableVertexAttribArray(matrix+3);    //matrix        8
        //        
        //        glEnableVertexAttribArray(texCoord);    //matrix        9
        //
        //	// vertex
        //	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[0]);
        //	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        //
        //        //pad
        //        //glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
        //        glVertexAttribPointer(pos+1, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)(sizeof(glm::vec3)));
        //
        //        //normal
        //	glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[1]);
        //	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        //        
        //        //explosion
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[2]);
        //        glVertexAttribPointer(explosion, 1, GL_FLOAT, GL_FALSE, 0, NULL);
        //        glVertexAttribDivisor(explosion, 1);
        //
        //        //color
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[3]);
        //        glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        //        glVertexAttribDivisor(color, 1);
        //        
        //
        //        //model matrix
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[4]);
        //        glVertexAttribPointer(matrix, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 0));
        //        glVertexAttribDivisor(matrix, 1);
        //        glVertexAttribPointer(matrix+1, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 1));
        //        glVertexAttribDivisor(matrix+1, 1);
        //        glVertexAttribPointer(matrix+2, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 2));
        //        glVertexAttribDivisor(matrix+2, 1);
        //        glVertexAttribPointer(matrix+3, 4, GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * 3));
        //        glVertexAttribDivisor(matrix+3, 1);
        //        
        //        glBindBuffer(GL_ARRAY_BUFFER, m_models[index]->buffers[5]);
        //	glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
        //
        //
        //        glBindVertexArray(0); // disable VAO

    }
    glUseProgram(0);

    check_gl_error();

    int err = glGetError();

    std::cout << "Loadmodel finish: " << _path;
    if (err)
        std::cout << "\033[31m with error: " << err;
    std::cout << "\n\033[30m";
}

void GLGraphics::LoadTexture(std::string _path) {
    m_texManager.Load2DTexture(_path, GL_TEXTURE2);
}

void GLGraphics::Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height) {
    if (m_TextureInstances.find(_id) != m_TextureInstances.end()) {
        return;
    }
    glUseProgram(m_shader2Dprogram);
    m_texManager.Load2DTexture(_path, GL_TEXTURE2);
    m_TextureInstances.insert(pair<int, TextureInfo*>(_id, new TextureInfo(m_texManager.GetTexture(_path), _x, _y, _width, _height)));

    if (m_TextureInstances.size() <= 1) {
        //buffra datan till 2D shader
        float positionData[] = {
            -1.0, -1.0,
            -1.0, 1.0,
            1.0, -1.0,
            -1.0, 1.0,
            1.0, 1.0,
            1.0, -1.0
        };

        float texCoordData[] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };

        int nrOfPoints = 12;

        GLuint m_2DVBOs[2];
        glGenBuffers(2, m_2DVBOs);

        // "Bind" (switch focus to) first buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, nrOfPoints * sizeof (float), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, nrOfPoints * sizeof (float), texCoordData, GL_STATIC_DRAW);

        // create 1 VAO
        glGenVertexArrays(1, &m_2DVAO);
        glBindVertexArray(m_2DVAO);

        // enable "vertex attribute arrays"
        glEnableVertexAttribArray(0); // position
        glEnableVertexAttribArray(1); // texCoord

        // map index 0 to position buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[0]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);

        glBindBuffer(GL_ARRAY_BUFFER, m_2DVBOs[1]);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);


        glBindVertexArray(0); // disable VAO
        glUseProgram(0); // disable shader programme
        //glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(2, m_2DVBOs);

        int err = glGetError();

        std::cout << "Add2DTexture ";
        if (err)
            std::cout << "\033[31m with error: " << err;
        std::cout << "\n\033[30m";
    }
}

void GLGraphics::Remove2DTexture(int _id) {
    delete(m_TextureInstances[_id]);
    m_TextureInstances.erase(_id);
}

void GLGraphics::Update() {

}

void GLGraphics::Resize(int _width, int _height) {
    m_screenWidth = _width;
    m_screenHeight = _height;
    glViewport(0, 0, m_screenWidth, m_screenHeight);
}

void GLGraphics::Free() {
    for (int i = m_models.size() - 1; i > -1; i--) {
        m_models[i]->instances.clear();
        //glDeleteBuffers(6,&m_models[i]->bufferVAOID);
        m_models.pop_back();
    }

    for (int i = m_testMatrices.size() - 1; i >-1; i--) 
    {
        m_testMatrices.pop_back();
    }

    glDeleteTextures(1, &m_computeTexture);
    glDeleteBuffers(1, &m_computeQuad);

    for (std::map<int, LightInfo*>::iterator it = m_lights.begin(); it != m_lights.end(); ++it) {
        delete(it->second);
    }
    m_lights.clear();

    for (std::map<int, TextureInfo*>::iterator it = m_TextureInstances.begin(); it != m_TextureInstances.end(); ++it) {
        delete(it->second);
    }
    m_TextureInstances.clear();
    m_texManager.Free();
    glDeleteVertexArrays(1, &m_2DVAO);

    glDeleteProgram(m_program);

    for (int i = 0; i < m_letters.size(); i++) {
        m_letters.pop_back();
    }

    for (int i = 0; i < m_textObjects.size(); i++) {
        m_textObjects.pop_back();
    }

    printf("Graphics memory cleared\n");
}

float t;

void GLGraphics::Render(ICamera* _camera) {
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_program);

    glViewport(0, 0, m_screenWidth, m_screenHeight-16.0);

    UpdateLights();

    CameraToRender(_camera);

    RenderCompute();

    //Render2D();

    for (int i = 0; i < m_textObjects.size(); i++) 
    {
        RenderText(m_textObjects[i].text, m_textObjects[i].scale, m_textObjects[i].color, m_textObjects[i].x, m_textObjects[i].y);
    }

    SDL_GL_SwapBuffers();
    

}

void GLGraphics::RenderText(std::string* _text, float* _scale, unsigned int* _color, int* _x, int* _y) 
{
    GLvoid* v;

    std::string text = (*_text);
    float scale = (*_scale);
    unsigned int color = (*_color);
    int x = (*_x);
    int y = (*_y);

    glPixelZoom(scale, scale);
    for (int i = 0; i < text.size(); i++) {
        v = (GLvoid*) m_letters[text.at(i) - 32];
        glRasterPos2i(x + i * 8 * scale, y * scale);
        glDrawPixels(8, 8, color, GL_BYTE, v);
    }
}

void GLGraphics::AddTextObject(std::string* _text, float* _scale, unsigned int* _color, int* _x, int* _y) 
{
    TextObject textObject;
    textObject.text = _text;
    textObject.scale = _scale;
    textObject.color = _color;
    textObject.x = _x;
    textObject.y = _y;

    m_textObjects.push_back(textObject);
}

void GLGraphics::RenderCompute() {
    t += 5.0f;
    
    glUseProgram(m_computeProgram);

//    for(int i=0; i< m_models.size();i++)
//    {
//        for(int j=0; j < m_models[i]->instances.size();j++)
//        {
//            glm::mat4* temp1;
//            
////            if(m_models[i]->instances[j]->world != NULL)
////            temp1 = m_models[i]->instances[j]->world;
////            else
//                temp1 = &m_testMatrices[0];
//
//            std::string matrixStr = "Models[0].worldMatrix";
//
//            GLint projection = glGetUniformLocation(m_computeProgram, matrixStr.c_str());
//            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(*temp1));
//        }
//    }
    
    glBindImageTexture(0, m_computeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    glDispatchCompute(32, 32, 1);
    glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);

    glUseProgram(m_program);
    glBindVertexArray(m_computeQuad);
    glDrawArrays(GL_QUADS, 0, 4);

    glUseProgram(0);

}

void GLGraphics::Render2D() {
    //printf("Entering Render2D");
    glUseProgram(m_shader2Dprogram);

    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    for (std::map<int, TextureInfo*>::iterator it = m_TextureInstances.begin(); it != m_TextureInstances.end(); ++it) {
        glBindVertexArray(m_2DVAO);
        glBindTexture(GL_TEXTURE_2D, it->second->TexHandle);
        //set viewPort (resterande TextureInfo-variabler)
        glViewport((GLint) (*it->second->X * m_screenWidth), (GLint) (*it->second->Y * m_screenHeight), (GLsizei) (*it->second->Width * m_screenWidth), (GLsizei) (*it->second->Height * m_screenHeight));
        //printf("X: %d  Y: %d    Width: %d    Height: %d \n\n", (GLint)(*it->second->X * m_screenWidth), (GLint)(*it->second->Y * m_screenHeight), (GLsizei)(*it->second->Width * m_screenWidth), (GLsizei)(*it->second->Height * m_screenHeight));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glActiveTexture(0);
}

int GLGraphics::RenderStandard() 
{
//    for(int i=0;i<m_models.size();i++)
//    {
//        glBindVertexArray(m_models[i]->bufferVAOID);
//
//        glDrawArrays(GL_TRIANGLES,0,m_models[i]->vertices);
//
//        glBindVertexArray(0);
//    }
}

int GLGraphics::RenderInstanced() {
    //    t+=0.001f;
    //
    //    ModelRenderInfo* MRI;
    //    
    //    glUseProgram(m_program);
    //    glEnable(GL_DEPTH_TEST);
    //    
    //    for(int i=0; i< m_models.size();i++)
    //    {
    //        MRI = m_models[i];
    //        int instances = m_models[i]->instances.size();
    //        
    //        glActiveTexture(GL_TEXTURE0);
    //        glBindTexture(GL_TEXTURE_2D, m_models[i]->texHandle);
    //        
    //        //Update matrix buffer//
    //        glBindBuffer(GL_ARRAY_BUFFER,m_models[i]->buffers[4]);
    //        glm::mat4* matrices = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    //        int j = 0;
    //
    //        for (std::map < int, ModelInstance*>::const_iterator insIt = m_models[i]->instances.begin(); insIt != m_models[i]->instances.end(); ++insIt)
    //        {
    //            matrices[j] = *insIt->second->world;
    //            //PRINTMATRIX(&matrices[j]);
    //            j++;
    //        }
    //        glUnmapBuffer(GL_ARRAY_BUFFER);
    //        glBindBuffer(GL_ARRAY_BUFFER,0);
    //        //Update matrix buffer//
    //        
    //        //Update explosion buffer//
    //
    //        glBindBuffer(GL_ARRAY_BUFFER,m_models[i]->buffers[2]);
    //        float* explosion = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    //
    //        j = 0;
    //
    //        for (std::map < int, ModelInstance*>::const_iterator insIt = m_models[i]->instances.begin(); insIt != m_models[i]->instances.end(); ++insIt)
    //        {
    //            if(insIt->second->explosion)
    //                explosion[j] = *insIt->second->explosion;
    //            else
    //                explosion[j] = 0.0f;
    //
    //            j++;
    //        }
    //        glUnmapBuffer(GL_ARRAY_BUFFER);
    //        glBindBuffer(GL_ARRAY_BUFFER,0);
    //        //Update explosion buffer<//
    //
    //        glBindVertexArray(MRI->bufferVAOID);
    //
    //        glDrawArraysInstanced(GL_TRIANGLES,0,MRI->vertices,instances);
    //
    //        glBindVertexArray(0);
    //        
    //    }

    return 1;
}

void GLGraphics::AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range) {
    m_lights.insert(pair<int, LightInfo*>(_id, new LightInfo(_worldPos, _intensity, _color, _range)));
}

void GLGraphics::RemovePointLight(int _id) {
    delete(m_lights[_id]);
    m_lights.erase(_id);
}

void GLGraphics::UpdateLights() {
    int i = 0;
    for (std::map<int, LightInfo*>::iterator it = m_lights.begin(); it != m_lights.end(); ++it) {
        //Light properties
        vec4 LightPosition = vec4(*it->second->Position, 1.0f); // Light position

        //-----Send all the lights values------
        const char* indexStr = std::to_string(i).c_str(); //itoa(i, indexStr, 10);
        char positionStr[25], intensityStr[25], colorStr[25], rangeStr[25];
        strcpy(positionStr, "Lights[");
        strcat(positionStr, indexStr);
        strcat(positionStr, "].Position");
        strcpy(intensityStr, "Lights[");
        strcat(intensityStr, indexStr);
        strcat(intensityStr, "].Intensity");
        strcpy(colorStr, "Lights[");
        strcat(colorStr, indexStr);
        strcat(colorStr, "].Color");
        strcpy(rangeStr, "Lights[");
        strcat(rangeStr, indexStr);
        strcat(rangeStr, "].Range");

        SetUniformV(m_computeProgram, positionStr, LightPosition);
        SetUniformV(m_computeProgram, intensityStr, *it->second->Intensity);
        SetUniformV(m_computeProgram, colorStr, *it->second->Color);
        SetUniformV(m_computeProgram, rangeStr, *it->second->Range);

        i++;
    }
}

void GLGraphics::CameraToRender(ICamera* _camera) 
{
    
    glm::mat4* temp1 = _camera->GetProjection();
    GLint projection = glGetUniformLocation(m_computeProgram, "m_matProj");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(*temp1));
    
    temp1 = _camera->GetView();
    GLint view = glGetUniformLocation(m_computeProgram, "m_matView");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(*temp1));
}

void GLGraphics::AddRenderObject(std::string _path, MATRIX4 _world) {

}

void GLGraphics::AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose, float* _explosion) {
    if (m_models.size() > 0)
        return;

    //    for(int i=0; i < m_models.size();i++)
    //    {
    //        if (m_models[0]->instances.count(_id) !=0)
    //                return;
    //    }

    int newModelID = -1;

    //    for(int i=0; i < m_models.size();i++)
    //    {
    //        if(m_models[i]->name == _model)
    //        {
    //            newModelID = i;
    //            int size = 0;
    //            
    //            //Resize instance buffers only
    //            glBindBuffer(GL_ARRAY_BUFFER, m_models[i]->buffers[2]); 
    //            glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
    //            glBufferData(GL_ARRAY_BUFFER, size + sizeof(float), NULL, GL_DYNAMIC_DRAW);
    //
    //            glBindBuffer(GL_ARRAY_BUFFER, m_models[i]->buffers[3]);
    //            glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
    //            glBufferData(GL_ARRAY_BUFFER, size + 4 * sizeof(float), NULL, GL_STATIC_READ);
    //
    //            glBindBuffer(GL_ARRAY_BUFFER, m_models[i]->buffers[4]);
    //            glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
    //            glBufferData(GL_ARRAY_BUFFER, size + 4 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    //
    //            glBindBuffer(GL_ARRAY_BUFFER,0);
    //            break;
    //        }
    //    }

    if (newModelID == -1) {
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

void GLGraphics::RemoveObject(int _id) {
    for (int i = 0; i < m_models.size(); i++) {
        m_models[i]->instances.erase(_id);
    }
}

int GLGraphics::SetUniformV(GLuint shaderProg, const char* _variable, float _value) {
    //	Set as current program
    glUseProgram(shaderProg);

    //	Get pointer for variable
    int location = glGetUniformLocation(shaderProg, _variable);
    if (location >= 0)
        glUniform1fv(location, 1, &_value);
    else
        return 1;

    return 0;
}

int GLGraphics::SetUniformV(GLuint shaderProg, const char* _variable, glm::vec3 _value) {
    //	Set as current program
    glUseProgram(shaderProg);

    //	Get pointer for variable
    int location = glGetUniformLocation(shaderProg, _variable);
    if (location >= 0)
        glUniform3fv(location, 1, &_value[0]);
    else
        return 1;

    return 0;
}

int GLGraphics::SetUniformV(GLuint shaderProg, const char* _variable, glm::vec4 _value) {
    //	Set as current program
    glUseProgram(shaderProg);

    //	Get pointer for variable
    int location = glGetUniformLocation(shaderProg, _variable);
    if (location >= 0)
        glUniform4fv(location, 1, &_value[0]);
    else
        return 1;

    return 0;
}

int GLGraphics::SetUniformV(GLuint shaderProg, const char* _variable, glm::mat3 _value) {
    //	Set as current program
    glUseProgram(shaderProg);

    //	Get pointer for variable
    int location = glGetUniformLocation(shaderProg, _variable);
    if (location >= 0)
        glUniformMatrix3fv(location, 1, GL_FALSE, &_value[0][0]);
    else
        return 1;

    return 0;
}

int GLGraphics::SetUniformV(GLuint shaderProg, const char* _variable, glm::mat4 _value) {
    //	Set as current program
    glUseProgram(shaderProg);

    //	Get pointer for variable
    int location = glGetUniformLocation(shaderProg, _variable);
    if (location >= 0)
        glUniformMatrix4fv(location, 1, GL_FALSE, &_value[0][0]);
    else
        return 1;

    return 0;
}

int GLGraphics::SetUniformV(GLuint shaderProg, const char* _variable, int _value) {
    //	Set as current program
    glUseProgram(shaderProg);

    //	Get pointer for variable
    int location = glGetUniformLocation(shaderProg, _variable);
    if (location >= 0)
        glUniform1i(location, _value);
    else return 1;

    return 0;
}

void GLGraphics::LoadLetters() {
    m_letters.push_back(&_space);
    m_letters.push_back(&_exclamation);
    m_letters.push_back(&_quote);
    m_letters.push_back(&_number);
    m_letters.push_back(&_dollar);
    m_letters.push_back(&_percent);
    m_letters.push_back(&_ampersand);
    m_letters.push_back(&_apostrophe);
    m_letters.push_back(&_leftbrace);
    m_letters.push_back(&_rightbrace);
    m_letters.push_back(&_asterisk);
    m_letters.push_back(&_plus);
    m_letters.push_back(&_comma);
    m_letters.push_back(&_minus);
    m_letters.push_back(&_dot);
    m_letters.push_back(&_slash);

    m_letters.push_back(&_0);
    m_letters.push_back(&_1);
    m_letters.push_back(&_2);
    m_letters.push_back(&_3);
    m_letters.push_back(&_4);
    m_letters.push_back(&_5);
    m_letters.push_back(&_6);
    m_letters.push_back(&_7);
    m_letters.push_back(&_8);
    m_letters.push_back(&_9);

    m_letters.push_back(&_colon);
    m_letters.push_back(&_semicolon);
    m_letters.push_back(&_lessthan);
    m_letters.push_back(&_equal);
    m_letters.push_back(&_morethan);
    m_letters.push_back(&_question);
    m_letters.push_back(&_at);

    m_letters.push_back(&A);
    m_letters.push_back(&B);
    m_letters.push_back(&C);
    m_letters.push_back(&D);
    m_letters.push_back(&E);
    m_letters.push_back(&F);
    m_letters.push_back(&G);
    m_letters.push_back(&H);
    m_letters.push_back(&I);
    m_letters.push_back(&J);
    m_letters.push_back(&K);
    m_letters.push_back(&L);
    m_letters.push_back(&M);
    m_letters.push_back(&N);
    m_letters.push_back(&O);
    m_letters.push_back(&P);
    m_letters.push_back(&Q);
    m_letters.push_back(&R);
    m_letters.push_back(&S);
    m_letters.push_back(&T);
    m_letters.push_back(&U);
    m_letters.push_back(&V);
    m_letters.push_back(&W);
    m_letters.push_back(&X);
    m_letters.push_back(&Y);
    m_letters.push_back(&Z);


}

void _check_gl_error(const char *file, int line) {
    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
        string error;

        switch (err) {
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM: error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE: error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        cerr << "GL_" << error.c_str() << " - " << file << ":" << line << endl;
        err = glGetError();
    }
}


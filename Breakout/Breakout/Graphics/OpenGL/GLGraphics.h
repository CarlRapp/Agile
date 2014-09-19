#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "../IGraphics.h"
#include "GLWindow.h"
#include "GLShader.h"

#include <GL/glew.h> 
#include <GL/gl.h> 
#include <GL/glu.h>

#include <SDL/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <map>
//#include <string>

class GLGraphics : public IGraphics
{
    struct ModelInstance
    {
        private:

        public:

        glm::mat4	*world;
        glm::mat4	*worldInverseTranspose;
    };
    
    struct ModelRenderInfo
    {
        public:
            
        int vertices;
        GLuint bufferVAOID;
        std::string name;
        std::map<int, ModelInstance*> instances;
        //std::vector<ModelInstance> instances;
        
        ModelRenderInfo(){}
      
    };
    
private:
	GLWindow *m_window;
        int m_screenWidth;
        int m_screenHeight;
        
        GLuint m_program;
        
        GLint m_attributePosition, m_attributeNormal;
        
        std::vector<ModelRenderInfo*> m_models;
        std::vector<Shader*> m_shaders;
        
        glm::vec3 m_testLightPos = glm::vec3(0,3,0);
                                            
        std::vector<glm::mat4> m_testMatrices;
        glm::vec4 m_testColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        
        //std::map<int, ModelInstance*> m_modelInstances;
        
//        int SetUniformV(const char* variable,float value);
//        int SetUniformV(const char* variable,glm::vec3 value);
//        int SetUniformV(const char* variable,glm::vec4 value);
//        int SetUniformV(const char* variable,glm::mat3 value);
//        int SetUniformV(const char* variable,glm::mat4 value);
//        int SetUniformV(const char* variable,int value);
        
        int ModelToRender();
        void LightsToRender();
        void CameraToRender(ICamera* _camera);
public:

	GLGraphics(void);
	~GLGraphics(void);

	bool  InitWindow(int x, int y, int width, int height, DisplayMode _displayMode);
        void Render(ICamera* _Camera);
        bool Init3D(DisplayMode _displayMode);
        void Resize(int width, int height);
        void Free();
        void Update();
        void LoadModel(std::string _path);
        
        void AddRenderObject(std::string _path, MATRIX4 _world);
        void AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose);
        void RemoveObject(int _id);
};



#endif
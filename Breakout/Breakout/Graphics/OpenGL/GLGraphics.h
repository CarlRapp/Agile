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
#include <string>
#include <stdio.h>
#include <stdlib.h>

using glm::vec3;
using glm::vec4;

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
    
    struct LightInfo
    {
        LightInfo(vec3 pos, vec3 intens, vec3 col, float range)
        {
            Position = pos;
            Intensity = intens;
            Color = col;
            Range = range;
        }
        vec3 Position; // Light position world coords.
	vec3 Intensity; // La, Ld and Ls intensity
	vec3 Color;
	float Range;
    };
    
private:
	GLWindow *m_window;
        int m_screenWidth;
        int m_screenHeight;
        
        GLuint m_program; //shaderID
        
        GLint m_attributePosition, m_attributeNormal;
        
        std::vector<ModelRenderInfo*> m_models;
        std::vector<Shader*> m_shaders;
        
        std::vector<LightInfo*> m_lights;
                                            
        std::vector<glm::mat4> m_testMatrices;
        glm::vec4 m_testColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        
        //std::map<int, ModelInstance*> m_modelInstances;
        
        int SetUniformV(GLuint shaderProg, const char* variable,float value);
        int SetUniformV(GLuint shaderProg, const char* variable,glm::vec3 value);
        int SetUniformV(GLuint shaderProg, const char* variable,glm::vec4 value);
        int SetUniformV(GLuint shaderProg, const char* variable,glm::mat3 value);
        int SetUniformV(GLuint shaderProg, const char* variable,glm::mat4 value);
        int SetUniformV(GLuint shaderProg, const char* variable,int value);
        
        int RenderInstanced();
        int RenderStandard();
        void AddLight(vec3 worldPos, vec3 intensity, vec3 color, float range);
        void UpdateLights();
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
#ifndef GLGRAPHICS_H
#define GLGRAPHICS_H

#include "../IGraphics.h"
#include "GLWindow.h"
#include "GLShaderHandler.h"
#include "GLTextureManager.h"
#include "GLText.h"
#include "GLParticleSystem.h"

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

            float   *explosion;
        glm::mat4	*world;
        glm::mat4	*worldInverseTranspose;
    };
    
    struct ModelRenderInfo
    {
        public:
            
        int vertices;
        GLuint bufferVAOID;
        GLuint texHandle;
        std::string name;
        std::map<int, ModelInstance*> instances;
        
        GLuint buffers[6];
        
        ModelRenderInfo(){}
      
    };
    
    struct LightInfo
    {
        LightInfo(vec3 *pos, vec3 *intens, vec3 *col, float *range)
        {
            Position = pos;
            Intensity = intens;
            Color = col;
            Range = range;
        }
        vec3 *Position; // Light position world coords.
	vec3 *Intensity; // La, Ld and Ls intensity
	vec3 *Color;
	float *Range;
    };
    
    struct TextObject
    {
        std::string* text;
        float* scale;
        unsigned int* color;
        int* x;
        int* y;
        float effectTime;
        int id;
        bool kill;
    };
    
private:
	GLWindow *m_window;
        int m_screenWidth;
        int m_screenHeight;
        
        GLTextureManager m_texManager;
        ShaderHandler m_standardShaderProgram, m_shader2Dprogram, m_particleShaderProgram; //shaderID
        GLuint m_2DVAO;
        
        GLParticleSystem *m_particlesFire;
        
        GLint m_attributePosition, m_attributeNormal;
        
        std::vector<ModelRenderInfo*> m_models;
        
        std::map<int, LightInfo*> m_lights;
                                            
        std::vector<glm::mat4> m_testMatrices;
        
        std::map<int, TextureInfo*> m_TextureInstances;
        
        std::vector<GLbyte(*)[64]> m_letters;
        
        std::vector<TextObject> m_textObjects;
        
        float m_textFontSize = 3;
        
        
        int RenderInstanced();
        int RenderStandard();
        void Render2D();
        void RenderParticles(int dt, int et, ICamera* _camera);
        void UpdateLights();
        void CameraToRender(ICamera* _camera);
        void LoadLetters();
        void RenderText(std::string* _text,float* _scale, unsigned int* _color,int* _x, int* _y,float effect,bool kill);
public:

	GLGraphics(void);
	~GLGraphics(void);

	bool  InitWindow(int x, int y, int width, int height, DisplayMode _displayMode);
        void Render(ICamera* _Camera);
        bool Init3D(DisplayMode _displayMode);
        void Resize(int width, int height);
        void Free();
        void Update(float _dt);
        void LoadModel(std::string _path);
        void LoadTexture(std::string _path);
        
        void AddPointLight(int _id, VECTOR3 *_worldPos, VECTOR3 *_intensity, VECTOR3 *_color, float *_range);
        void RemovePointLight(int _id);
        
        void Add2DTexture(int _id, std::string _path, float *_x, float *_y, float *_width, float *_height);
        void Remove2DTexture(int _id);
        
        void AddRenderObject(std::string _path, MATRIX4 _world);
        void AddObject(int _id, std::string _model, MATRIX4 *_world, MATRIX4 *_worldInverseTranspose,float* _explosion);
        void RemoveObject(int _id);
        
        
        void AddTextObject(std::string* _text,float* _scale, unsigned int* _color,int* _x,int* _y,int _id);
        void RemoveTextObject(int _id);
        

};



#endif
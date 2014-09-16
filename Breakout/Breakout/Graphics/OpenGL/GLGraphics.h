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
//#include <string>

class GLGraphics : public IGraphics
{

private:
	GLWindow *m_window;
        int m_screenWidth;
        int m_screenHeight;
        
        GLuint vbo_cube_vertices, vbo_cube_colors;
        GLuint ibo_cube_elements;
        GLuint m_program;
        
        GLint uniform_mvp;
        GLint attribute_coord3d, attribute_v_color;
        
        std::vector<Shader*> m_shaders;
        
        glm::vec3 m_testLightPos = glm::vec3(0,3,0);
        glm::mat4 m_testMatrix = glm::mat4 {1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1};
        
        int SetUniformV(const char* variable,float value);
        int SetUniformV(const char* variable,glm::vec3 value);
        int SetUniformV(const char* variable,glm::vec4 value);
        int SetUniformV(const char* variable,glm::mat3 value);
        int SetUniformV(const char* variable,glm::mat4 value);
        int SetUniformV(const char* variable,int value);
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
};



#endif
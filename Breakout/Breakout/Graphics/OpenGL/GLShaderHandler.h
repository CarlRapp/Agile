#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	//GLuint GetShaderID() { return m_ShaderID; }
	//GLenum GetShaderTYPE() { return m_ShaderTYPE; }
        void CreateShaderProgram();
	bool AddShader(std::string _path, GLenum _shaderType);
        void LinkShaderProgram();
        
        void UseProgram();
        
        GLuint GetProgramHandle(){ return m_program; }
        
        //lägg till alla setUniforms
        int SetUniformV(const char* variable,float value);
        int SetUniformV(const char* variable,glm::vec3 value);
        int SetUniformV(const char* variable,glm::vec4 value);
        int SetUniformV(const char* variable,glm::mat3 value);
        int SetUniformV(const char* variable,glm::mat4 value);
        int SetUniformV(const char* variable,int value);
private:
	std::vector<GLuint> m_shaders;
	GLenum	m_program;
        
};

#endif
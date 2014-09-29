#ifndef SHADER_H
#define SHADER_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

class Shader
{
public:
	Shader(std::string _path, GLenum _shaderType);
	~Shader();

	GLuint GetShaderID() { return m_ShaderID; }
	GLenum GetShaderTYPE() { return m_ShaderTYPE; }
	bool LoadShader(std::string _path, GLenum _shaderType);

private:
	GLuint	m_ShaderID;
	GLenum	m_ShaderTYPE;
        
};

#endif
#include "GLShader.h"
#include <fstream>
#include <unistd.h>
#include <stdio.h>

#define SHADER_ROOT "/Graphics/OpenGL/GLShaders/"
char    m_cwd[FILENAME_MAX];

Shader::Shader(std::string _path, GLenum _shaderType)
{
	m_ShaderID = -1;
        LoadShader(_path, _shaderType);
}

Shader::~Shader()
{

}
bool Shader::LoadShader(std::string _path, GLenum _shaderType)
{
    std::ifstream fileStream;
    
    if (!getcwd(m_cwd, sizeof(m_cwd)))
    {
        printf ("Unable to find current working directory!\n");
        return false;
    }
    
	//	Open the file
    std::string	finalString = SHADER_ROOT;
    std::string	workingDir = m_cwd;
    
    workingDir = workingDir.append(finalString);
    _path = workingDir.append(_path);
    finalString = "";
    
    fileStream.open(_path.c_str(),std::ifstream::in);

    //	Check if the file is good
    if (fileStream.is_open())
    {
            std::string	s;

            //	Loop through the file and 
            //	append the finalString with
            //	the files content
            while (std::getline(fileStream, s))
            {
                    s.push_back('\n');
                    finalString.append(s);
            }

            fileStream.close();
    }
    else
    {
            printf("Unable to find shader path:%s!\n", _path.c_str());
            return false;
    }

    //	If the string was empty
    //	don't continue
    if (finalString.empty())
            return false;

    //	Create the shader and set the
    //	shader type
    m_ShaderID = glCreateShader(_shaderType);
    m_ShaderTYPE = _shaderType;

    //	Set the shader source to the created shader
    //	and try to compile it
    const GLchar*	shaderSource = (char*)finalString.c_str();

    glShaderSource(m_ShaderID, 1, &shaderSource, NULL);

    glCompileShader(m_ShaderID);
  
    

    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &vShaderCompiled);

    if (vShaderCompiled != GL_TRUE)
    {
            printf("Unable to compile shader %d!\n", m_ShaderID);
            return false;
    }
    //printf("ERROR %d\n",glGetError());
    printf("Shader load success %s\n", _path.c_str());

    return true;
}
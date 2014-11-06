#include "GLShader.h"
#include "../../stdafx.h"
#include <fstream>

#include <stdio.h>


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
    
//    if (!getcwd(m_cwd, sizeof(m_cwd)))
//    {
//        printf ("Unable to find current working directory!\n");
//        return false;
//    }
//    
//	//	Open the file
//    std::string	finalString = SHADER_ROOT;
//    std::string	workingDir = m_cwd;
//    
//    workingDir = workingDir.append(finalString);
//    _path = workingDir.append(_path);
    std::string finalString = "";
    
    fileStream.open(GetFile(_path.c_str(),SHADER_ROOT),std::ifstream::in);

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
            printf("\033[31mUnable to find shader path:%s!\n\033[30m", _path.c_str());
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
        int length = 0;
        glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length);
        if(length > 0) 
        {
                // create a log of error messages
                char* errorLog = new char[length];
                int written = 0;
                glGetShaderInfoLog(m_ShaderID, length, &written, errorLog);
                printf("Shader error log;\n%s\n", errorLog);
                delete [] errorLog;
        }
        
        printf("\033[31mUnable to compile shader: %s\n\033[30m",_path.c_str());

        return false;
    }

    printf("\033[32mShader load success %s\n\033[30m", _path.c_str());

    return true;
}
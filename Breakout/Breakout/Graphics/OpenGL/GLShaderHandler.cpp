#include "GLShaderHandler.h"
#include "../../stdafx.h"
#include <fstream>

#include <stdio.h>
#include <vector>


ShaderHandler::ShaderHandler()
{
}

ShaderHandler::~ShaderHandler()
{
    /* REMOVE PROGRAM */
    glDeleteProgram( m_program );

    /* REMOVE SHADERS */
    for ( int i = 0; i < m_shaders.size( ); i++ )
    {
        glDeleteShader( m_shaders[i] );
    }
}

void ShaderHandler::CreateShaderProgram()
{
    m_program = glCreateProgram();
}

bool ShaderHandler::AddShader(std::string _path, GLenum _shaderType)
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
    GLuint shaderID = glCreateShader(_shaderType);
   // m_ShaderTYPE = _shaderType;

    //	Set the shader source to the created shader
    //	and try to compile it
    const GLchar*	shaderSource = (char*)finalString.c_str();

    glShaderSource(shaderID, 1, &shaderSource, NULL);

    glCompileShader(shaderID);
  
    

    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &vShaderCompiled);

    if (vShaderCompiled != GL_TRUE)
    {
        int length = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        if(length > 0) 
        {
                // create a log of error messages
                char* errorLog = new char[length];
                int written = 0;
                glGetShaderInfoLog(shaderID, length, &written, errorLog);
                printf("Shader error log;\n%s\n", errorLog);
                delete [] errorLog;
        }
        
        printf("\033[31mUnable to compile shader: %s\n\033[30m",_path.c_str());

        return false;
    }

    printf("\033[32mShader load success %s\n\033[30m", _path.c_str());
    
    glAttachShader(m_program, shaderID);
    m_shaders.push_back(shaderID);

    return true;
}

void ShaderHandler::LinkShaderProgram()
{
    glLinkProgram(m_program);
    
    /* ERROR HANDELING */
    GLint success = 0;
    glGetProgramiv( m_program, GL_LINK_STATUS, (int*)&success );

    if ( success == GL_FALSE )
    {
            /* GET THE SIZE OF THE ERROR LOG */
            GLint logSize = 0;
            glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &logSize );

            /* GET THE ERROR LOG*/
            GLchar *errorLog;
            errorLog = new GLchar[logSize];
            glGetProgramInfoLog( m_program, logSize, &logSize, &errorLog[0] );

            /* PRINT THE ERROR LOG */
            for ( int i = 0; i < logSize; i++ )
            {
                    printf( "%c", errorLog[i] );
            }
            printf("\n");

            /* DO SOME CLEANING :) */
            delete(errorLog);

            /* REMOVE PROGRAM */
            glDeleteProgram( m_program );

            /* REMOVE SHADERS */
            for ( int i = 0; i < m_shaders.size( ); i++ )
            {
                    glDeleteShader( m_shaders[i] );
            }
            m_shaders.clear( );
            return;
    }
}

void ShaderHandler::UseProgram()
{
    glUseProgram(m_program);
}

int ShaderHandler::SetUniformV(const char* _variable, float _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform1fv(location, 1, &_value);
	else
		return 1;

	return 0;
}

int ShaderHandler::SetUniformV(const char* _variable, glm::vec3 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform3fv(location, 1, &_value[0]);
	else
		return 1;

	return 0;
}

int ShaderHandler::SetUniformV(const char* _variable, glm::vec4 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform4fv(location, 1, &_value[0]);
	else
		return 1;

	return 0;
}

int ShaderHandler::SetUniformV(const char* _variable, glm::mat3 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniformMatrix3fv(location, 1, GL_FALSE, &_value[0][0]);
	else 
		return 1;

	return 0;
}

int ShaderHandler::SetUniformV(const char* _variable, glm::mat4 _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniformMatrix4fv(location, 1, GL_FALSE, &_value[0][0]);
	else
		return 1;

	return 0;
}

int ShaderHandler::SetUniformV(const char* _variable, int _value)
{
	//	Set as current program
	glUseProgram(m_program);

	//	Get pointer for variable
	int location = glGetUniformLocation(m_program, _variable);
	if (location >= 0)
		glUniform1i(location, _value);
	else return 1;

	return 0;
}